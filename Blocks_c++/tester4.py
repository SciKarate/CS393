print("help")

print("newfs hello.dev 512 512")
print("mount hello.dev")
print("iNodeMapFull")
print("setiNode 2\nsetiNode 5")
i = 3
while (i > 0):
	print("allociNode")
	i = i - 1
print("freeiNode 3\nsetiNode 3\nfreeiNode 4\nfreeiNode 5")
print("iNodeMapFull")
print("unmount")
print("mount hello.dev")
print("iNodeMapFull")
print("unmount")

print("quit")