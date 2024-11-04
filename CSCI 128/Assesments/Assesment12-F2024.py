from helper_library import image_to_list, show_image, output_image

def block_average(grid, x, y, width, height):
    r, g, b = 0, 0, 0
    pixels = width * height
    for i in range(x, x + width):
        for j in range(y, y + height):
            r += grid[j][i][0]
            g += grid[j][i][1]
            b += grid[j][i][2]
    
    return [r//pixels, g//pixels, b//pixels]

def create_compressed_block(avg_color, width, height):
    block = []
    for j in range(height):
        row = [avg_color] * width
        block.append(row)

    return block

def merge_lists(lst1, lst2):
    for index,row in enumerate(lst1):
        lst1[index] = row + lst2[index]

    return lst1

def compress_image(grid, x, y, width, height, threshold):
    if width <= threshold or height <= threshold:   
        return create_compressed_block(block_average(grid, x, y, width, height), width, height)

    half_width = width // 2
    half_height = height // 2

    top_left = compress_image(grid, x, y, half_width, half_height, threshold)  
    top_right = compress_image(grid, x + half_width, y, width - half_width, half_height, threshold)  
    bottom_left = compress_image(grid, x, y + half_height, half_width, height - half_height, threshold) 
    bottom_right = compress_image(grid, x + half_width, y + half_height, width - half_width, height - half_height, threshold) 

    return merge_lists(top_left,top_right) + merge_lists(bottom_left,bottom_right)

if __name__ == "__main__":
    image_name = input("IMAGE_FILENAME> ")

    grid = image_to_list(image_name)
    output_image_file = f'compressed_{image_name}'
    threshold = int(input("COMPRESSION_THRESHOLD> "))

    width = len(grid[0])
    height = len(grid)


    print(f'OUTPUT Width: {width}')
    print(f'OUTPUT Height: {height}')
    print(f'OUTPUT Threshold: {threshold}')

    compressed_grid = compress_image(grid, 0, 0, width, height, threshold)
    output_image(compressed_grid, output_image_file)
    show_image(compressed_grid)
