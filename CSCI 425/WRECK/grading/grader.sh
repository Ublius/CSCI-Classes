#!/usr/bin/env bash

# unlike SIM projects, each compiler project has a name of its own
PROGRAM=WRECK

# this shifts off ${1:-.} to COMPLOC
source "${COMPGRADING}/comp-lib.sh"
source "${GRADERDIR}/grader-exec.sh"
set -e

# make executable (|| true allows invocation to fail gracefully on gradescope where
# the normscanu script is setup correctly but owned by grader.sh process)
chmod +x "${graderloc}/normscanu" || true
# look for required execs
for x in aenorm aenfatott cmptt ; do 
	if ! test -x "${COMPGRADING}/$x" ; then 
		grader_echo "ERROR: '${COMPGRADING}/$x' can be executed."
		exit 1
	fi
done

# create a temporary dir to hold tests, this permits multiple 
# simultaneous grading sessions without stumbling over each other...
GRADERBIO="_wrecktest"
mkdir -p "${GRADERBIO}"
mirror "${graderloc}/basicio" "${GRADERBIO}"
# some students need llre.cfg for run
test -s "llre.cfg" || cp "${graderloc}/llre.cfg" "."


NOTOKEN=_test_empty.scan-u_output_file

compare_scan_u()
{
	local tst="${1}" ; shift
	"${graderloc}/normscanu"  "${1}" >"_${tst}_student.u"
	"${graderloc}/normscanu"  "${2}" >"_${tst}_expected.u"
	if ! diff "_${tst}_student.u" "_${tst}_expected.u" >/dev/null ; then 
		grader_msg <<EOT
scan.u output file for test set ${tst} in '${graderloc}' FAILED".  The output from your ${PROGRAM} is in ${1};
it has been normalized to _${tst}_students.u and compared to _${tst}_expected.u.

You can see which lines differ using "visual diff" tool, or at the console with

  $ diff -u _${tst}_students.u _${tst}_expected.u

(- lines would be removed from _${tst}_students.u and + lines would be added to 
match _${tst}_expected.u).

IF YOU WANT TO INSPECT THIS FAILURE, CTRL-C now!
EOT
		return 1
	else
		rm -f "_${tst}_student.u" "_${tst}_expected.u"
	fi
	
}

compare_nfa()
{
	local tst="${1}" ; shift
	local tokid="${1}" ; shift
	local stalph=`head -n 1 "${1}" | ( read n l x ; echo "$x"; ) |tee "_comparenfa-sa.lis" | "${COMPGRADING}/aenorm"`	
	local exalph=`head -n 1 "${2}" | ( read n l x ; echo "$x"; ) |tee "_comparenfa-ea.lis" | "${COMPGRADING}/aenorm"`	
	local ret=0
	if ! test "${stalph}" = "${exalph}" ; then
		ret=1
		grader_msg <<EoT
The alphabet in the first line of '${1}' after normalization is
  ${stalph:-empty}
which does not compare favorably to the expected alphabet
  ${exalph}

IF YOU WANT TO INSPECT THIS FAILURE, CTRL-C now!
EoT
	fi
	${COMPGRADING}/aenfatott "${1}" >"_${tst}_${tokid}_student_dfa.tt"
	${COMPGRADING}/aenfatott "${2}" >"_${tst}_${tokid}_expected_dfa.tt"
	if ! ${COMPGRADING}/cmptt "_${tst}_${tokid}_expected_dfa.tt" "_${tst}_${tokid}_student_dfa.tt" 2>"_${tst}_${tokid}_dfacmp.err"; then
		ret=1
		grader_msg <<EoT
The NFA stored for tokenid ${tokid} (${1}) has been converted to an optimized
DFA at _${tst}_${tokid}_student_dfa.tt.  This should be the same as the DFA
in _${tst}_${tokenid}_expected_dfa.tt; but it does not compare favorably.

There *should* be PDFs of your NFA's initial, simplified and minimized
representations stored at  ${1%.nfa}-input.pdf,  ${1%.nfa}-simplified.pdf  and
${1%.nfa}-output.pdf.  The same style PDFs for ${tst} ${tokid} that generate
the grader script results are provided in an aptly named ${tst/test/result}/
directory alongside the grader.sh you are using.  Comparing these PDFs to yours
may provide a hint as to where your NFA construction routine isn't quite up to
snuff. KEEP IN MIND: the output pdfs should match precisely, the input pdfs not
so (since they are NFAs).

The  ${1%.nfa}-simplified  NFA  is the original input NFA but with irrelevant 
states removed.  I've learned it can be easier to see construction logic flaws
in the simplified form --- but the input NFA is what your WRECK is actually 
generating!

The diff(1) differences between the transition table files (.tt) are stored in
_${tst}_${tokid}_dfacmp.err, this may or may not be helpful since it can be a
long way between NFA and optimized DFA.

IF YOU WANT TO INSPECT THIS FAILURE, CTRL-C now!
EoT
	else 
		rm "_${tst}_${tokid}_student_dfa.tt"
		rm "_${tst}_${tokid}_expected_dfa.tt"
		rm "${1%.nfa}"-*.pdf 
		rm -f "${1%.nfa}"-*.gv
	fi
	return $ret
}

test_re_set()
{
	# $1=graderloc test set name
	# $2=tally prefix for scanu gen
	# $3=tally prefix for nfa gen
	local tallyu="${2}"
	local tallynfa="${3}"
	local testdir=_wrecktest
	rm -rf "${testdir}/${1}"
	mkdir -p "${testdir}/${1}"
	mirror "${graderloc}/${1}" "${testdir}/${1}/"
	# some students need llre.cfg for run
	test -s "llre.cfg" || cp "${graderloc}/llre.cfg" "."

	
	local nfaerr=0
	local badu="${tallyu}${1}.bad"
	if ! test_run "${comploc}/${PROGRAM}" REC llre.cfg "${testdir}/${1}/scan.lut" "_output.u" && test ${GraderRunES} -ne 0 ; then
		grader_echo "ERROR: exit status ${GraderRunES} non-zero."
		touch "${badu}"
	elif ! test -s ./_output.u; then
		grader_echo "ERROR: No ./_output.u generated."
		touch "${badu}"
	elif ! compare_scan_u ${1} ./_output.u "${graderloc}/${1/test/result}/scan.u" ; then
		touch "${badu}"
	else 
		touch "${badu%.bad}.good"
	fi

	for nfa in "${graderloc}/${1/test/result}/"*.nfa ; do 
		prognfa="${nfa##*/}"
		badn="${tallynfa}${1}${prognfa%.nfa}.bad"	
		if ! test -s "${prognfa}" ; then
			grader_echo "ERROR: missing ${prognfa}."
			touch "${badn}"
			nfaerr=1
		elif ! compare_nfa ${1} ${prognfa%.nfa} "${prognfa}" "${nfa}" ; then
			touch "${badn}"
			nfaerr=1
		else 
			touch "${badn%.bad}.good"
		fi
	done


	if test -f "${badu%.bad}.good" -a ${nfaerr} -eq 0 ; then
		grader_echo "RE compilation and scan.u generation for ${1} is GOOD :)"
		rm -rf "${testdir}/"
		rm -f  "_${1}"* _output.u
		for nfa in "${graderloc}/${1/test/result}/"*.nfa ; do 
			rm -f "${nfa##*/}"
		done
	else 
		grader_msg <<EoT
${1} test set failed --- you should have seen messages with details before this.
EoT
		grader_keystroke
	fi
	
	grader_echo ""
}

# REC tests
export basiciotallydir=`create_tally_dir 1 MAX=10 Exit status 1 on inassessible, unwritable or empty files`
export lexicaltallydir=`create_tally_dir 2 MAX=5 '(REC)' Exit status 4 on lexical errors`
export semantictallydir=`create_tally_dir 3 MAX=5 '(REC)' Exit status 3 on semantic  errors`
export syntaxtallydir=`create_tally_dir 4 MAX=5 '(REC)' Exit status 2 on syntax errors`
export tallyudir=`create_tally_dir 5 MAX=10 '(REC)' Generates correct scan.u`
export tallynfadir=`create_tally_dir 6 MAX=20 '(REC)' Generates correct tokenid.nfa NFA files`

REC()
{
	# RUBRIC 1 ###
	# inaccessible scan.lut
	tally_missing_datafile "${basiciotallydir}/missinglut" \
		"${comploc}/${PROGRAM}" REC llre.cfg MISSINGDATAFILE ${NOTOKEN} 

	# inaccessible output location
	rm -rf "${graderloc}/${GRADERBIO}/_dne"
	tally_nooutput_exitnonzero "${basiciotallydir}/nowriteu" \
		"${comploc}/${PROGRAM}" REC llre.cfg "${GRADERBIO}/_abc.lut" "${GRADERBIO}/_dne/_scan.u" 

	# unwritable _tokenid.nfa file
	nopermnfa=`sed -n 2p "${GRADERBIO}/_bad.lut" | awk '{print $2}'`.nfa
	# clear out
	rm -rf "${nopermnfa}"
	# unwritable file
	touch "${nopermnfa}"
	chmod -w "${nopermnfa}"
	tally_nooutput_exitnonzero "${basiciotallydir}/nopermnfaf" \
		"${comploc}/${PROGRAM}" REC llre.cfg "${GRADERBIO}/_bad.lut" "${GRADERBIO}/_scan.u"
	# should also fail w/ dirs
	rm -f "${nopermnfa}"
	mkdir -p "${nopermnfa}"
	tally_nooutput_exitnonzero "${basiciotallydir}/nopermnfad" \
		"${comploc}/${PROGRAM}" REC llre.cfg "${GRADERBIO}/_bad.lut" "${GRADERBIO}/_scan.u"
	rm -rf "${nopermnfa}"

	# RUBRIC 2-4 syntax and semantic errors
	for tsttype in lexical syntax semantic ; do 
		tdir=${tsttype}tallydir
		case ${tsttype} in lexical ) expes=4 ;; syntax ) expes=2 ;; semantic ) expes=3 ;; * ) expes=NaN ;; esac
		for tstlut in "${graderloc}/${tsttype}"*/scan.lut ; do 
			tid=${tstlut##*/*${tsttype}}
			rm -f "${GRADERBIO}/"_scan.* 
			cp "${tstlut}" "${GRADERBIO}/_scan.lut"
			tally_ignore_exitvalue "${!tdir}/${tsttype}${tid%/scan.lut}" -eq ${expes} \
				"${comploc}/${PROGRAM}" REC llre.cfg "${GRADERBIO}/_scan.lut" "${GRADERBIO}/_scan.u"
		done
	done

	rm -rf "${GRADERBIO}"

	for tst in `( cd "${graderloc}" && ls -1d test[0-9] )` ; do 
		test_re_set "${tst}" "${tallyudir}/" "${tallynfadir}/" 
	done
}

CHECK_rubrics()
{
	# CHECK tests --- install these only when CHECK mod tests are invoked
	export llttallydir=`create_tally_dir 7 MAX=10 '(CHECK)' Exit status 5 on LLT conflicts`
	export checksyntaxtallydir=`create_tally_dir 8 MAX=5 '(CHECK)' Exit status 2 on syntax errors`
	export countdepthtallydir=`create_tally_dir 9 MAX=10 '(CHECK)' COUNTDEPTH output`
}

CHECK()
{
	CHECK_rubrics 

	# RUBRIC 1 ###
	# inaccessible cfg and source
	tally_missing_datafile "${basiciotallydir}/missingcfg" \
		"${comploc}/${PROGRAM}" CHECK MISSINGDATAFILE /dev/null 3

	tally_missing_datafile "${basiciotallydir}/missingsrc" \
		"${comploc}/${PROGRAM}" CHECK "${graderloc}/check-m/isll-m.cfg" MISSINGDATAFILE 3

	# Not LL(1) - exit status 5
	for notll in ${graderloc}/check-*/notll-*.cfg ; do
		cp "${notll}" _cfg
		tid=${notll##*/}
		tid=${tid%.cfg}
		rm -f _src && touch _src
		grader_msg <<EoT
LL(1) grammar checking "${graderloc}/${PROGRAM}" with ${tid}.cfg 
EoT
		tally_ignore_exitvalue "${llttallydir}/${tid}" -eq 5 \
			"${comploc}/${PROGRAM}" CHECK _cfg _src 3
	done	
	rm -f _cfg _src

	# Syntax error - exit status 2
	for syn in ${graderloc}/check-*/syntax-*.src; do
		case "${syn}" in 
			*\*.src) 
				#echo >&2 'wt?:  no syntax checks for CHECK mode?'
				continue
				;;
		esac
		Z=${syn##*/}
		tid=${Z%.src}
		Z=${Z#syntax-}
		Z=${Z%-*}
		grader_msg <<EoT
Syntax checking "${graderloc}/${PROGRAM}" with isll-${Z}.cfg and ${syn##*/}
EoT
		cp "${syn%/*}/isll-${Z}.cfg" _cfg
		cp "${syn}" _src
		tally_ignore_exitvalue "${checksyntaxtallydir}/${tid}" -eq 2 \
			"${comploc}/${PROGRAM}" CHECK _cfg _src 3
	done	
	rm -f _cfg _src

	
	# COUNTDEPTH
	for cc in ${graderloc}/check-*/*-*-*.src; do
		case "${cc}" in 
			*\*.src) 
				#echo >&2 'wt?:  no COUNTDEPTH checks for CHECK mode?'
				continue
				;;
			*/syntax-*src ) continue;;
		esac
		Z=${cc##*/}
		tid=${Z%.src}
		Z=${Z%%-*}
		CC=${tid#${Z}-}
		CC=${CC%%-*}
		#echo cc $cc Z $Z tid $tid
		cp "${cc%/*}/isll-${Z}.cfg" _cfg
		cp "${cc}" _src
		rm -f _ccresult
		touch "${countdepthtallydir}/${tid}.bad"

		grader_msg <<EoT
Running ...
  "${comploc}/PROGRAM" CHECK "${cc%/*}/isll-${Z}.cfg" "${cc}" ${CC}
EoT
		# what a hack (see test_run doc in grader-lib.sh, geesh)
		if ( ! test_run -Q "${comploc}/${PROGRAM}" CHECK _cfg _src ${CC} ) && \
	         ( test_run -Q "${comploc}/${PROGRAM}" CHECK _cfg _src ${CC} |\
			 "${COMPGRADING}/output-pipe" > ${comploc}/_ccresult ) ; then
			if diff "${comploc}/_ccresult" "${graderloc}/check-${Z}/${tid}.lis" >/dev/null ; then
				grader_msg <<EoT
Running ${PROGRAM} on isll-${Z}.cfg and ${cc##*/} with COUNTDEPTH parameter ${CC} looks good! :)
EoT
				mv "${countdepthtallydir}/${tid}.bad"  "${countdepthtallydir}/${tid}.good"
			fi
		fi
		if test -f "${countdepthtallydir}/${tid}.bad" -a -f "${comploc}/_ccresult" ; then
			local ccres=`tr '\n' ' ' <"${comploc}/_ccresult"`
			local ccexp=`tr '\n' ' ' <"${graderloc}/check-${Z}/${tid}.lis"`
			grader_msg <<EoT
ERROR ${PROGRAM} on isll-${Z}.cfg and ${cc%%*/} with COUNTDEPTH parameter ${CC}...

This notice may have been caused by an unexpected exit status (see above), or the 
COUNTDEPTH result was /${ccres% }/ while the expected result was /${ccexp% }/.
EoT
			grader_keystroke
		fi
	done	
	rm -f _cfg _src _ccresult
}

case "${GRADERMODE}" in
	REC|"" ) REC ;;
	CHECK )  CHECK ;;
	ALL ) REC; CHECK ;;
esac

show_tallies

