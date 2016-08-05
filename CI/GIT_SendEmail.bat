@echo off  
E:
cd E:\GIT\PlanGit\CI
git log -1 > 1.txt
start python mail_post.py  
exit  