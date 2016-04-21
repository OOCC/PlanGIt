@echo off  
E:
ping -n 10 127.0.0.1
echo git log -1

cd E:\GIT\PlanGit\CI
start python mail_post.py  
exit  