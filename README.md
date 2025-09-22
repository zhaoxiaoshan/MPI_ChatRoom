# MPI_ChatRoom
用OpenMPI技术做的一个聊天室
更新记录：
2025.9.22: 
	a. 发布版本1.0.0；
	b. 将右上角的“关闭”按钮隐藏；
	c. 修改BUG，当点击“退出”时，当前节点和其他节点都能优雅的退出;
	d. 用Isend/Recv API替代了Bcast，因为广播接口和send/recv接口不兼容；
