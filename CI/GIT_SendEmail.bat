@echo off  
E:
ping -n 3 127.0.0.1
unset $(git rev-parse --local-env-vars)
cd E:\GIT\PlanGit\CI
start python mail_post.py  
exit  