""" basic smoke test
    tests a 1-block blockmap
    creates a file system
    mounts it
    allocates stuff
    frees every other one
    unmounts
    mounts again
    allocates again - shouldn't get an already allocated block
    unmount
"""

print("newfs testpy.dev 256 100");
print("mount testpy.dev");
for i in range(100):
    print("allocBlock");
print("blockMap");
for i in range(50):
    print("freeBlock ", 2*i);
print("allocINode");
print("allocINode");
print("unmount");
print("mount testpy.dev");
print("blockMap");
print("allocBlock");
print("allocBlock");
print("allocBlock");
print("allocINode");
print("allocINode");
print("unmount");

