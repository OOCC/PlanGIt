@echo off  
E:  
unset $(git rev-parse --local-env-vars)
cd E:\GIT\PlanGit\CI
start python mail_post.py  
exit  