class Node:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None
        self.size = 1  

def get_size(node):
    return node.size if node else 0

def update_size(node):
    if node:
        node.size = 1 + get_size(node.left) + get_size(node.right)

def split(node, k):
    if not node:
        return None, None
    
    left_size = get_size(node.left)
    
    if k <= left_size:
        small, large = split(node.left, k)
        node.left = large
        update_size(node)
        return small, node
    else:
        small, large = split(node.right, k - left_size - 1)
        node.right = small
        update_size(node)
        return node, large