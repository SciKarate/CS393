"""
   test filesystem
"""
print("newfs hello.dev 2048 128")
print("mount hello.dev")
print("ls")
i = 0
while(i < 5):
	print("mkdir dirsibling" + str(i))
	i += 1
i = 0
while(i < 9):
	print("mkdir dirchild" + str(i))
	if(i == 5):
		print("mkdir dirchildsib" + str(i))
	print("cd dirchild" + str(i))
	i += 1
i = 0
while(i < 9):
	print("cd ..")
	i += 1
print("ls")
print("unmount")
print("mount hello.dev")
print("ls")
i = 0
while(i < 9):
	print("cd dirchild" + str(i))
	print("ls")
	i += 1

print("quit")