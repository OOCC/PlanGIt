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
    ps = os.popen("git log -1")
    uline = []
    global Str
    global context
    context = ''
    Str = ''
    print(u"one")
    for line in ps:
        print(u"tsss")
        uline.append(line.decode('utf-8'))
        #.decode('utf-8')
        print uline


        print(u"two")

    print(u"three")
    for conStr in uline:
        context = context + conStr
        print '111 = ' + context
    print 'conStr = ' + context
    send_mail(u'xueyu push post', context)

def send_mail(sub,content): 
    ############# 
    #要发给谁，这里发给1个人 
    mailto_list=["xueuy@qq.com, 635541030@qq.com, 982224886@qq.com"] 
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
    msg = MIMEText(content, _charset='gbk')
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