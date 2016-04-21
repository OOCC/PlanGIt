#coding: gb2312
import sys
import os
import smtplib
from email.mime.text import MIMEText

mail_host = 'smtp.example.com' #�����ʼ���smtp��ַ
mail_user = 'example' # ����֪ͨ�ʼ����û���
mail_pass = 'example_passwd' # �û�������
me = 'Example' + '<' + 'example' + '@' + 'example.com' + '>' #�����ʼ��˵ĵ�ַ��ʶ
to_list = ['person_a@example.com', 'person_b@example.com', 'person_c@example.com'] # �ռ���

html_template = """
<html>
        <h2 style="color:#FFFFFF; background: #008040;">������Ϣ</h2>
        <div> <b>�汾�⣺</b>
                <a href="svn:%s">%s</a>
        </div>
        <div> <b>�汾�ţ�</b>%s
        </div>
        <div>
                <b>�ύ�ߣ�</b>%s
        </div>
        <div>
                <b>�ύʱ�䣺</b>%s
        </div>
        <h2 style="color:#FFFFFF; background: #4682B4;">�ύ˵��</h2> <font size="4" color="#BF6000"><xmp>%s</xmp></font>
        <h2 style="color:#FFFFFF; background: #5353A8;">�ļ��嵥</h2>
        <xmp>%s</xmp>
        <hr>
        <center>
                �� Powered by
                <a href="http://garyelephant.me">Gary</a>
        </center>
        <center>
                �� Inspired by
                <a href="http://crearo-sw.blogspot.com">CREARO-SW</a>
        </center>
</html>
"""

def write_mail_content(repo, rev):
        """
        repo: repository
        rev: revision
        """
        repo_name = get_repo_name(repo)
        author = get_author(repo, rev)
        date = get_date(repo, rev)
        log = get_log(repo, rev)
        file_list = get_file_list(repo, rev)
        content = html_template % (repo, repo_name, rev, author, date, log, file_list
        return content

def get_repo_name(repo):
        return os.path.basename(repo)

def get_author(repo, rev):
        """svnlook author -r REV REPOS ����ύ��
        """
        cmd = '%s author -r %s %s' % (svnlook_bin_path, rev, repo)
        output = os.popen(cmd).read()
        return output

def get_date(repo, rev):
        """svnlook date -r REV REPOS ����ύʱ��
        """
        cmd = '%s date -r %s %s' % (svnlook_bin_path, rev, repo)
        output = os.popen(cmd).read()
        return output

def get_log(repo, rev):
        """svnlook log -r REV REPOS ����ύ��־
        """
        cmd = '%s log -r %s %s' % (svnlook_bin_path, rev, repo)
        output = os.popen(cmd).read()
        return output

def get_file_list(repo, rev):
        """svnlook changed -r REV REPOS ��÷���������ļ�
        """
        cmd = '%s changed -r %s %s' % (svnlook_bin_path, rev, repo)
        output = os.popen(cmd).read()
        return output

def send_mail(msg, sender, to_list):
        try:
                s = smtplib.SMTP()
                s.connect(mail_host)
                s.login(mail_user,mail_pass)
                s.sendmail(sender, to_list, msg.as_string())
                s.close()
                return True
        except Exception, e:
                print str(e)
                return False

def write_mail(sender, to_list, sub, content):
        msg = MIMEText(content, _subtype = 'html', _charset='gb2312')
        msg['Subject'] = sub
        msg['From'] = sender
        msg['To'] = ';'.join(to_list)
        return msg

global svnlook_bin_path
if __name__ == '__main__':
        svnlook_bin_path = '/usr/local/subversion/bin/svnlook'

        subject = 'SVN Commit Notification'
        content = write_mail_content(sys.argv[1], sys.argv[2])
        msg = write_mail(me, to_list, subject, content)
        send_mail(msg, me, to_list)