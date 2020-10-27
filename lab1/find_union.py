class Element:
    def __init__(self, key):
        self.parent = None
        self.rank = 0
        self.size = 1
        self.id = key


def find(el):
    if el.parent is None:
        return el
    el.parent = find(el.parent)

    return el.parent


def union(x, y):
    x_root = find(x)
    y_root = find(y)

    if x_root.rank > y_root.rank:
        y_root.parent = x_root
        x_root.size += y_root.size
    elif y_root.rank > x_root.rank:
        x_root.parent = y_root
        y_root.size += x_root.size
    elif x_root != y_root:
        y_root.parent = x_root
        x_root.rank += 1
        x_root.size += y_root.size


def sizeOfSet(x):
    return find(x).size


