def remove_comments(text: str) -> str:
    result = []
    i, n = 0, len(text)
    in_block = in_line = in_string = False
    while i < n:
        if in_string:
            if text[i] == '"':
                in_string = False
            result.append(text[i])
            i += 1
        elif in_block:
            if text[i] == '*' and i+1 < n and text[i+1] == '/':
                in_block = False
                i += 2
            else:
                i += 1
        elif in_line:
            if text[i] == '\n':
                in_line = False
                result.append(text[i])
            i += 1
        else:
            if text[i] == '/' and i+1 < n:
                if text[i+1] == '/':
                    in_line = True
                    i += 2
                    continue
                elif text[i+1] == '*':
                    in_block = True
                    i += 2
                    continue
            if text[i] == '"':
                in_string = True
            result.append(text[i])
            i += 1
    return "".join(result)