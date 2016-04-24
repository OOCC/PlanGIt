#!/usr/bin/env python 
# -*- coding: utf-8 -*- 
#导入smtplib和MIMEText 
import smtplib,sys 
from email.mime.text import MIMEText
from subprocess import Popen, PIPE 
import os
import re

def disk_space():
    ## take shell command output
    ps = os.popen("git log")

    uline = []
    i = 0
    print(u"one")
    for line in ps:
        line = line.strip()
        uline.append(line.decode('utf-8'))
        print uline[i]
        print(u"two")
        i += 1
    str = '\n'.join(uline)
    print(u"three")
    send_mail(u'xueyu push post',str)

def send_mail(sub,content): 
    ############# 
    #要发给谁，这里发给1个人 
    mailto_list=["xueuy@qq.com"] 
    ##################### 
    #设置服务器，用户名、口令以及邮箱的后缀 
    mail_host="smtp.126.com" 
    mail_user="xy741" 
    mail_pass="xy123858" 
    mail_postfix="126.com" 
    ###################### 
    ''''' 
    to_list:发给谁 
    sub:主题 
    content:内容 
    send_mail("aaa@126.com","sub","content") 
    ''' 
    me=mail_user+"<"+mail_user+"@"+mail_postfix+">" 
    msg = MIMEText(content,_charset='gbk') 
    msg['Subject'] = sub 
    msg['From'] = me 
    msg['To'] = ";".join(mailto_list) 
    try: 
        s = smtplib.SMTP() 
        s.connect(mail_host) 
        s.login(mail_user,mail_pass) 
        s.sendmail(me, mailto_list, msg.as_string()) 
        s.close() 
        return True 
    except Exception, e: 
        print str(e) 
        return False 
if __name__ == '__main__': 
    if disk_space(): 
        print '发送中' 
    else: 
        print '发送完毕'
    raw_input()