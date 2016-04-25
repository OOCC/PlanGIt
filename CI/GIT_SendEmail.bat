@echo off  
E:
ping -n 1 127.0.0.1
git log -1 > 1.txt

cd E:\GIT\PlanGit\CI
start python mail_post.py  
exit  