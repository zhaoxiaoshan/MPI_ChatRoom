# MPI_ChatRoom
## 用OpenMPI技术做的一个聊天室<br>
### 依赖环境
&nbsp;&nbsp;&nbsp;&nbsp;1. 开发环境依赖Microsoft MPI SDK;<br>
&nbsp;&nbsp;&nbsp;&nbsp;2. 运行环境依赖Microsoft MPI;<br>
&nbsp;&nbsp;&nbsp;&nbsp;3. 依赖软件的下载地址：https://www.microsoft.com/en-us/download/details.aspx?id=57467<br>
### 更新记录：<br>
<b>2025.9.22:</b> <br>
&nbsp;&nbsp;&nbsp;&nbsp;1. 发布版本1.0.0；<br>
&nbsp;&nbsp;&nbsp;&nbsp;2. 将右上角的“关闭”按钮隐藏；<br>
&nbsp;&nbsp;&nbsp;&nbsp;3. 修改BUG，当点击“退出”时，当前节点和其他节点都能优雅的退出;<br>
&nbsp;&nbsp;&nbsp;&nbsp;4. 用Isend/Recv API替代了Bcast，因为广播接口和send/recv接口不兼容；<br>
