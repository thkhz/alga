def solve(n):
    def backtrack(row, queens, cols, diag1, diag2):
        if row == n:
            result.append(queens)
            return

        for col in range(n):
            if col not in cols and (row - col) not in diag1 and (row + col) not in diag2:
                board_row = "." * col + "Q" + "." * (n - col - 1)
                
                backtrack(row + 1, 
                          queens + [board_row], 
                          cols | {col}, 
                          diag1 | {row - col}, 
                          diag2 | {row + col})

    result = []
    backtrack(0, [], set(), set(), set())
    return result

n = int(input())
solutions = solve(n)

if solutions:
    for row in solutions[0]:
        print(row)