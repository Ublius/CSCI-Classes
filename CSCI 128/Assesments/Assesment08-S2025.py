import string

def dot_product(vector_1, vector_2):
    return sum([vector_1[i] * vector_2[i] for i in range(len(vector_1))])

def magnitude(vector):
    return sum([vector[i] ** 2 for i in range(len(vector))]) ** 0.5

def cosine(vector_1, vector_2):
    return dot_product(vector_1, vector_2) / (magnitude(vector_1) * magnitude(vector_2))

def words_to_vector(index_array, word_array):
    return [word_array.count(index) for index in index_array]

def join_frequency_vectors(frequency_vector_1, frequency_vector_2):
    return [frequency_vector_1[i] + frequency_vector_2[i] for i in range(len(frequency_vector_1))]

def get_document():
    return [input(f"LINE {i+1}> ").split() for i in range(int(input("NUM LINES> ")))]

def clean_document_line(line):
    clean = []

    for word in line:
        if '-' in word:
            clean.extend(clean_document_line(word.split('-')))
            continue
        if '_' in word:
            clean.extend(clean_document_line(word.split('_')))
            continue

        new_word = ''

        for char in word:
            if char in string.punctuation or char in string.digits:
                continue

            new_word += char
        
        clean.append(new_word.lower())
     
    return list(filter(None, clean))

def create_index_array(doc_1, doc_2):
    seen = set()
    return [word for line in doc_1 + doc_2 for word in line if not (word in seen or seen.add(word))]

if __name__ == '__main__':
    doc_1_raw = get_document()
    doc_2_raw = get_document()

    doc_1 = []
    doc_2 = []

    for line in doc_1_raw:
        line = clean_document_line(line)
        if len(line) == 0:
            continue

        doc_1.append(line)

    for line in doc_2_raw:
        line = clean_document_line(line)
        if len(line) == 0:
            continue

        doc_2.append(line)
    if doc_1 and doc_2:
        index_array = create_index_array(doc_1, doc_2)

        doc_1_vector = words_to_vector(index_array, doc_1[0])
        for i in range(1, len(doc_1)):
            doc_1_vector = join_frequency_vectors(doc_1_vector, words_to_vector(index_array, doc_1[i]))

        doc_2_vector = words_to_vector(index_array, doc_2[0])
        for i in range(1, len(doc_2)):
            doc_2_vector = join_frequency_vectors(doc_2_vector, words_to_vector(index_array, doc_2[i]))

        similarity = cosine(doc_1_vector, doc_2_vector) * 100

        print(f"OUTPUT {similarity:.2f}%")

        if similarity > 75:
            print("OUTPUT Very Similar")
        elif similarity > 50:
            print("OUTPUT Similar")
        elif similarity > 25:
            print("OUTPUT Not Very Similar")
        else:
            print("OUTPUT Not Similar")
    else:
        print("OUTPUT ERROR: At least one document is empty!")
