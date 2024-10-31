# Assessment 8 
import helper_module

def get_input_grid():
    num_inputs = 0
    grid = []
    while num_inputs < 5:
        #ask for user input
        row = input(f"ROW{num_inputs}> ")
        #check that input is not done
        if row == "DONE":
            # return -1
            return -1
        # convert the input to a list
        line = row.split()
        # convert the list data to integers 
        for i in range(len(line)):
            line[i] = int(line[i])
        # add it to the pixel grid
        grid.append(line)
        # increment the pixel number
        num_inputs += 1
    # return the 2D grid of pixels
    return grid

def grid_is_valid(pixel_grid):
    for i in range(5):
        # check that the grid is a list of 5 lists
        if(len(pixel_grid[i]) != 5):
            return False
        for num in pixel_grid[i]:
            # check that each nested list has 5 integers with only a 0 or 1 value
            if num != 1 and num != 0:
                return False
    # return true
    return True

def match_score(pixel_grid, letter_grid):
    match_count = 0
    # for each row in the grid
    for i in range(len(pixel_grid)):
        # for each column in the grid
        for j in range(len(pixel_grid)):
            # check if the values are the same
            if pixel_grid[i][j] == letter_grid[i][j]:
                # increase match count
                match_count += 1
    # return the number of matches
    return match_count

def get_all_scores(pixel_grid, letter_grids):
    score_list = []
    # for each letter grid in the list of grids
    for i in range(len(letter_grids)):
        # get the number of matches
        score = match_score(pixel_grid, letter_grids[i])
        # add the score to the score list
        score_list.append(score)
    # return the score list 
    return score_list

if __name__ == "__main__":
    while True:
        # take the user input
        input_grid = get_input_grid()
        # check that it is valid
        if input_grid == -1:
            print("OUTPUT DONE entered, program exiting")
            break
        # call grid_is_valid and make sure it works
        if (grid_is_valid(input_grid) == False):
            print("OUTPUT ERROR: invalid grid")
            continue
        # use the helper module to get letter grid data
        grids_of_letters = helper_module.get_grids()
        letters_list = helper_module.get_letters()
        # call get_all_scores() to get the score list
        list_of_match_scores = get_all_scores(input_grid, grids_of_letters)
        # find the highest score
        best_score = max(list_of_match_scores)
        # get the letter that corresponds to the best score
        best_letter = letters_list[list_of_match_scores.index(best_score)]
        # print the match and match % 
        print(f'OUTPUT Best match: {best_letter}, Match %: {(best_score / 25) * 100:.0f}%')

