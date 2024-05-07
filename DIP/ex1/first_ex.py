# Task 1
def mySum(i: int, j: int) -> int:
    if i > j:
        return 0
    else:
        return i + mySum(i+1, j)

# Task 2
def pascal(row: int, column: int) -> int:

    if column == 0:
        return 1
    elif row == 0:
        return 0
    else:
        return pascal(row-1, column) + pascal(row-1, column-1)


# Task 3
def find(list, to_be_found):
    for char in list:
        if char == to_be_found:
            return True

def balance(chars: list) -> bool:

    if len(chars) == 0:
        return True
    else:
        char = chars[0]
        if char != "(" and char != ")":
            chars.pop(chars.index(char))
            print(chars)
        elif char == "(":
            found = find(chars, ")")
            if found:
                chars.pop(chars.index(char))
                chars.pop(chars.index(")"))
                print(chars)

        balance(chars)
    return False

# It does not work I know, too tired to figure this simple thing ou


