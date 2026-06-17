n = int(input())
if n == 0:
    print("true")
else:
    scores = list(map(int, input().split()))
    total_sum = sum(scores)

    if total_sum % 2 != 0:
        print("false")
    else:
        target = total_sum // 2
        possible_sums = {0}
        
        for score in scores:
            new_sums = set()
            for s in possible_sums:
                if s + score <= target:
                    new_sums.add(s + score)
            possible_sums.update(new_sums)
        
        if target in possible_sums:
            print("true")
        else:
            print("false")