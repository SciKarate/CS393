print("help")

print("newfs hello.dev 128 128")
print("mount hello.dev")
print("blockMapFull")
print("setBlock 78\nsetBlock 99\nsetBlock 124\nsetBlock 111\nsetBlock 0")
i = 64
while (i > 0):
	print("allocBlock")
	i = i - 1
print("freeBlock 37\nsetBlock 38\nfreeBlock 38\nsetBlock 38")
print("blockMapFull")
print("unmount")
print("mount hello.dev")
print("blockMapFull")
print("unmount")

print("quit")