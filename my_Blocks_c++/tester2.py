print("help")

print("newfs hello.dev 32 64")
print("mount hello.dev")
print("blockMapFull")
print("setBlock 18\nsetBlock 54\nsetBlock 62\nsetBlock 37\nsetBlock 0")
print("allocBlock\nallocBlock\nallocBlock\nallocBlock\nallocBlock")
print("freeBlock 37\nsetBlock 38\nfreeBlock 38\nsetBlock 38")
print("blockMapFull")
print("unmount")
print("mount hello.dev")
print("blockMapFull")
print("unmount")

print("quit")