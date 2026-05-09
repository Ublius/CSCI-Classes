# default GRADERMODE is UGRAD for the undergrad projects,
# graduate students can change this to ALL in this file or
# invoke the grader script with GRADERMODE=ALL in the environment
test -z "${GRADERMODE}" && GRADERMODE=ALL
export GRADERMODE
