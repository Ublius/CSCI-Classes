use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Write};
use std::process;
use std::collections::{BTreeSet, HashMap, VecDeque};

type StateId = usize;

struct NFA {
    lambda_index: usize,
    transitions: Vec<Vec<Vec<StateId>>>,
    accepting: Vec<bool>,
    alphabet: Vec<u8>,
    num_states: usize,
}

struct DFA {
    transitions: Vec<Vec<Option<StateId>>>,
    accepting: Vec<bool>,
}

fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 3 {
        eprintln!("Usage: NFAMATCH <nfa_def_file> <dfa_output> <test_token1> [<test_token2> ...]");
        process::exit(1);
    }

    let nfa_file = &args[1];
    let dfa_output_file = &args[2];
    let tokens = &args[3..];

    let nfa = load_nfa_definition(nfa_file)?;
    
    let dfa = nfa_to_dfa(&nfa);
    let optimized_dfa = optimize_dfa(&dfa);
    
    write_dfa(&optimized_dfa, dfa_output_file)?;
    
    for token in tokens {
        test_token(&optimized_dfa, token, &nfa.alphabet);
    }
    
    Ok(())
}

// ============ Helper Functions ============
fn decode_value(s: &str) -> Vec<u8> {
    let mut result = Vec::new();
    let bytes = s.as_bytes();
    let mut i = 0;
    
    while i < bytes.len() {
        if i + 2 < bytes.len() && (bytes[i] == b'x' || bytes[i] == b'X') {
            if let Ok(byte) = u8::from_str_radix(&s[i+1..i+3], 16) {
                result.push(byte);
                i += 3;
                continue;
            }
        }
        result.push(bytes[i]);
        i += 1;
    }
    
    result
}

fn follow_lambda(states: &BTreeSet<StateId>, nfa: &NFA) -> BTreeSet<StateId> {
    let mut result = states.clone();
    let mut stack: Vec<StateId> = states.iter().cloned().collect();
    
    while let Some(state) = stack.pop() {
        if state < nfa.transitions.len() {
            let lambda_idx = nfa.lambda_index;
            if lambda_idx < nfa.transitions[state].len() {
                for &next in &nfa.transitions[state][lambda_idx] {
                    if next < nfa.transitions.len() && result.insert(next) {
                        stack.push(next);
                    }
                }
            }
        }
    }
    
    result
}

fn write_dfa(dfa: &DFA, filename: &str) -> io::Result<()> {
    let mut file = File::create(filename)?;
    
    for (state_id, row) in dfa.transitions.iter().enumerate() {
        let sign = if dfa.accepting[state_id] { "+" } else { "-" };
        write!(file, "{} {}", sign, state_id)?;
        
        for &next in row {
            let s = match next {
                Some(n) => n.to_string(),
                None => "E".to_string(),
            };
            write!(file, " {}", s)?;
        }
        writeln!(file)?;
    }
    
    Ok(())
}

// ============ Load NFA ============
fn load_nfa_definition(filename: &str) -> io::Result<NFA> {
    let file = File::open(filename)?;
    let reader = BufReader::new(file);
    
    let mut lines: Vec<String> = reader.lines().filter_map(Result::ok).map(|l| l.trim().to_string()).filter(|l| !l.is_empty()).collect();
    
    if lines.is_empty() {
        return Err(io::Error::new(io::ErrorKind::InvalidData, "Empty NFA file"));
    }
    
    let first_line = lines.remove(0);
    let mut parts = first_line.split_whitespace();
    
    let num_states: usize = parts.next().unwrap().parse().unwrap();
    let lambda_char = decode_value(parts.next().unwrap())[0];
    
    let mut alphabet: Vec<u8> = vec![lambda_char];
    for part in parts {
        alphabet.extend(decode_value(part));
    }
    
    let lambda_index = 0;
    let alphabet_size = alphabet.len();
    
    let mut max_state = num_states - 1;
    for line in &lines {
        let fields: Vec<&str> = line.split_whitespace().collect();
        if fields.len() >= 3 {
            let from: usize = fields[1].parse().unwrap();
            let to: usize = fields[2].parse().unwrap();
            if from > max_state {
                max_state = from;
            }
            if to > max_state {
                max_state = to;
            }
        }
    }
    
    let actual_num_states = max_state + 1;
    
    let mut transitions = vec![vec![Vec::new(); alphabet_size]; actual_num_states];
    let mut accepting = vec![false; actual_num_states];
    
    for line in lines {
        let mut fields = line.split_whitespace();
        
        let flag = fields.next().unwrap();
        let from: usize = fields.next().unwrap().parse().unwrap();
        let to: usize = fields.next().unwrap().parse().unwrap();
        
        if flag == "+" {
            if from < accepting.len() {
                accepting[from] = true;
            }
        }
        
        for symbol_field in fields {
            let symbols = decode_value(symbol_field);
            for sym in symbols {
                if let Some(pos) = alphabet.iter().position(|&c| c == sym) {
                    if from < transitions.len() && pos < transitions[from].len() {
                        transitions[from][pos].push(to);
                    }
                }
            }
        }
    }
    
    Ok(NFA {
        lambda_index,
        transitions,
        accepting,
        alphabet,
        num_states: actual_num_states,
    })
}

// ============ NFA To DFA ============
fn nfa_to_dfa(nfa: &NFA) -> DFA {
    let alphabet_size = nfa.alphabet.len();
    let real_alphabet_size = if alphabet_size > 0 { alphabet_size - 1 } else { 0 };
    
    let mut start_set = BTreeSet::new();
    if 0 < nfa.num_states {
        start_set.insert(0);
    }
    let start_closure = follow_lambda(&start_set, nfa);
    
    let mut dfa_states: Vec<BTreeSet<StateId>> = Vec::new();
    let mut dfa_transitions: Vec<Vec<Option<StateId>>> = Vec::new();
    let mut dfa_accepting: Vec<bool> = Vec::new();
    let mut state_map: HashMap<BTreeSet<StateId>, StateId> = HashMap::new();
    let mut queue: VecDeque<BTreeSet<StateId>> = VecDeque::new();
    
    state_map.insert(start_closure.clone(), 0);
    dfa_states.push(start_closure.clone());
    dfa_transitions.push(vec![None; real_alphabet_size]);
    dfa_accepting.push(start_closure.iter().any(|&s| s < nfa.accepting.len() && nfa.accepting[s]));
    queue.push_back(start_closure);
    
    while let Some(current_set) = queue.pop_front() {
        let current_id = state_map[&current_set];
        
        for sym_idx in 1..alphabet_size {
            let mut next_set = BTreeSet::new();
            for &state in &current_set {
                if state < nfa.transitions.len() && sym_idx < nfa.transitions[state].len() {
                    for &next in &nfa.transitions[state][sym_idx] {
                        if next < nfa.transitions.len() {
                            next_set.insert(next);
                        }
                    }
                }
            }
            
            if next_set.is_empty() {
                continue;
            }
            
            let closure = follow_lambda(&next_set, nfa);
            
            let next_id = if let Some(&id) = state_map.get(&closure) {
                id
            } else {
                let new_id = dfa_states.len();
                state_map.insert(closure.clone(), new_id);
                dfa_states.push(closure.clone());
                dfa_transitions.push(vec![None; real_alphabet_size]);
                dfa_accepting.push(closure.iter().any(|&s| s < nfa.accepting.len() && nfa.accepting[s]));
                queue.push_back(closure);
                new_id
            };
            
            if sym_idx > 0 && sym_idx - 1 < dfa_transitions[current_id].len() {
                dfa_transitions[current_id][sym_idx - 1] = Some(next_id);
            }
        }
    }
    
    DFA {
        transitions: dfa_transitions,
        accepting: dfa_accepting,
    }
}

// ============ Optimize FA ============
fn optimize_dfa(dfa: &DFA) -> DFA {
    let pruned_dfa = prune_dfa(&dfa.transitions, &dfa.accepting);
    
    let mut current_transitions = pruned_dfa.transitions;
    let mut current_accepting = pruned_dfa.accepting;
    
    loop {
        let (new_transitions, new_accepting) = merge_states(&current_transitions, &current_accepting);
        
        if new_transitions.len() == current_transitions.len() {
            break;
        }
        
        current_transitions = new_transitions;
        current_accepting = new_accepting;
    }
    
    prune_dfa(&current_transitions, &current_accepting)
}

fn merge_states(transitions: &[Vec<Option<StateId>>], accepting: &[bool]) -> (Vec<Vec<Option<StateId>>>, Vec<bool>) {
    let num_states = transitions.len();
    if num_states <= 1 {
        return (transitions.to_vec(), accepting.to_vec());
    }
    
    let alphabet_size = transitions[0].len();
    
    let mut partitions: Vec<Vec<StateId>> = Vec::new();
    
    let accepting_states: Vec<StateId> = (0..num_states)
        .filter(|&i| accepting[i])
        .collect();
    if !accepting_states.is_empty() {
        partitions.push(accepting_states);
    }
    
    let non_accepting_states: Vec<StateId> = (0..num_states)
        .filter(|&i| !accepting[i])
        .collect();
    if !non_accepting_states.is_empty() {
        partitions.push(non_accepting_states);
    }
    
    let mut changed = true;
    while changed {
        changed = false;
        let mut new_partitions = Vec::new();
        
        for partition in &partitions {
            if partition.len() <= 1 {
                new_partitions.push(partition.clone());
                continue;
            }
            
            let mut split_occurred = false;
            
            for sym in 0..alphabet_size {
                if split_occurred {
                    break;
                }
                
                let mut group_map: HashMap<Option<StateId>, Vec<StateId>> = HashMap::new();
                
                for &state in partition {
                    let target = if let Some(next) = transitions[state][sym] {
                        let mut target_partition = None;
                        for (part_idx, part) in partitions.iter().enumerate() {
                            if part.contains(&next) {
                                target_partition = Some(part_idx);
                                break;
                            }
                        }
                        target_partition
                    } else {
                        None
                    };
                    
                    group_map.entry(target).or_insert_with(Vec::new).push(state);
                }
                
                if group_map.len() > 1 {
                    split_occurred = true;
                    changed = true;
                    
                    for (_, group) in group_map {
                        new_partitions.push(group);
                    }
                }
            }
            
            if !split_occurred {
                new_partitions.push(partition.clone());
            }
        }
        
        if changed {
            partitions = new_partitions;
        }
    }
    
    partitions.sort_by(|a, b| a[0].cmp(&b[0]));
    
    let mut old_to_new = vec![0; num_states];
    let mut new_accepting = Vec::new();
    
    for (new_id, partition) in partitions.iter().enumerate() {
        for &state in partition {
            old_to_new[state] = new_id;
        }
        new_accepting.push(partition.iter().any(|&s| accepting[s]));
    }
    
    let new_num_states = partitions.len();
    let mut new_transitions = vec![vec![None; alphabet_size]; new_num_states];
    
    for (new_id, partition) in partitions.iter().enumerate() {
        let representative = partition[0];
        for sym in 0..alphabet_size {
            if let Some(next) = transitions[representative][sym] {
                new_transitions[new_id][sym] = Some(old_to_new[next]);
            }
        }
    }
    
    (new_transitions, new_accepting)
}

fn prune_dfa(transitions: &[Vec<Option<StateId>>], accepting: &[bool]) -> DFA {
    let num_states = transitions.len();
    if num_states == 0 {
        return DFA {
            transitions: Vec::new(),
            accepting: Vec::new(),
        };
    }
    
    let alphabet_size = transitions[0].len();
    
    let mut reachable = vec![false; num_states];
    let mut queue = VecDeque::new();
    
    if 0 < num_states {
        reachable[0] = true;
        queue.push_back(0);
    }
    
    while let Some(state) = queue.pop_front() {
        for &next_opt in &transitions[state] {
            if let Some(next) = next_opt {
                if next < num_states && !reachable[next] {
                    reachable[next] = true;
                    queue.push_back(next);
                }
            }
        }
    }
    
    let mut live = vec![false; num_states];
    let mut rev_queue = VecDeque::new();
    
    for i in 0..num_states {
        if accepting[i] && reachable[i] {
            live[i] = true;
            rev_queue.push_back(i);
        }
    }
    
    let mut reverse_trans: Vec<Vec<StateId>> = vec![Vec::new(); num_states];
    for from in 0..num_states {
        if reachable[from] {
            for &next_opt in &transitions[from] {
                if let Some(to) = next_opt {
                    if to < num_states && reachable[to] {
                        reverse_trans[to].push(from);
                    }
                }
            }
        }
    }
    
    while let Some(state) = rev_queue.pop_front() {
        for &prev in &reverse_trans[state] {
            if reachable[prev] && !live[prev] {
                live[prev] = true;
                rev_queue.push_back(prev);
            }
        }
    }
    
    let mut useful = vec![false; num_states];
    for i in 0..num_states {
        useful[i] = reachable[i] && live[i];
    }
    
    if !useful[0] {
        return DFA {
            transitions: vec![vec![None; alphabet_size]],
            accepting: vec![false],
        };
    }
    
    let mut old_to_new = vec![None; num_states];
    let mut useful_states = Vec::new();
    
    for i in 0..num_states {
        if useful[i] {
            old_to_new[i] = Some(useful_states.len());
            useful_states.push(i);
        }
    }
    
    let useful_count = useful_states.len();
    let mut new_transitions = vec![vec![None; alphabet_size]; useful_count];
    let mut new_accepting = vec![false; useful_count];
    
    for (new_id, &old_id) in useful_states.iter().enumerate() {
        new_accepting[new_id] = accepting[old_id];
        
        for sym in 0..alphabet_size {
            if let Some(next) = transitions[old_id][sym] {
                if let Some(mapped_next) = old_to_new[next] {
                    new_transitions[new_id][sym] = Some(mapped_next);
                }
            }
        }
    }
    
    DFA {
        transitions: new_transitions,
        accepting: new_accepting,
    }
}

// ============ Compare Tokens ============
fn test_token(dfa: &DFA, token: &str, alphabet: &[u8]) {
    let bytes = token.as_bytes();
    let mut current_state = 0;
    
    if bytes.is_empty() {
        let accepting = *dfa.accepting.get(0).unwrap_or(&false);
        if accepting {
            println!("OUTPUT :M:");
        } else {
            println!("OUTPUT 0");
        }
        return;
    }
    
    for (pos, &c) in bytes.iter().enumerate() {
        let mut col = None;
        for (idx, &sym) in alphabet.iter().enumerate() {
            if idx > 0 && sym == c {
                col = Some(idx - 1);
                break;
            }
        }
        
        let col = match col {
            Some(c) => c,
            None => {
                println!("OUTPUT {}", pos + 1);
                return;
            }
        };
        
        if current_state >= dfa.transitions.len() || col >= dfa.transitions[current_state].len() {
            println!("OUTPUT {}", pos + 1);
            return;
        }
        
        match dfa.transitions[current_state][col] {
            Some(next) => {
                if next < dfa.transitions.len() {
                    current_state = next;
                } else {
                    println!("OUTPUT {}", pos + 1);
                    return;
                }
            }
            None => {
                println!("OUTPUT {}", pos + 1);
                return;
            }
        }
    }
    
    let accepting = *dfa.accepting.get(current_state).unwrap_or(&false);
    if accepting {
        println!("OUTPUT :M:");
    } else {
        println!("OUTPUT {}", bytes.len() + 1);
    }
}