d = dict()

with open('/Users/vladkirilov/CLionProjects/posix_threads/times') as file:
    i = 1
    for row in file:
        d[str(i)] = row
        i += 1

x_str = '['
y_str = '['
for i in d.items():
    x_str += f"\'{i[0]}\'"
    y_str += f"\'{i[1].strip()}\'"

    if int(i[0]) != len(d.keys()):
        x_str += ","
        y_str += ","


x_str += ']'
y_str += ']'

print(y_str)

with open('/Users/vladkirilov/CLionProjects/posix_threads/lab8/template.html') as file:
    string = file.read()
    string = string.replace("%x", x_str)
    string = string.replace("%y", y_str)

with open('/Users/vladkirilov/CLionProjects/posix_threads/lab8/graphics.html', 'w') as file:
    file.write(string)

# l = []
#
# with open('/Users/vladkirilov/CLionProjects/posix_threads/results') as file:
#     for string in file:
#         l.append(float(string) - 3.14159265358979323846)
#
# print(l)
#
# sortd = copy(l)
# sortd.sort()
#
# print(sortd)
#
# if l[0] > l[1]:
#     print(1)