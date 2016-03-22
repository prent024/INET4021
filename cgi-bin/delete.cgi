#!/usr/bin/python
import cgi
import os
import cgitb; cgitb.enable()

print "Content-Type: text/html\n\n"

form = cgi.FieldStorage()
def delete_file(form_field):
    if form.has_key(form_field):
        os.remove(form[form_field].value)
        os.remove("Pictures/" +form[form_field].value)

if form.has_key('Name'):
    picname = form['Name'].value
    print "<!DOCTYPE html>"
    print "<html>"
    print "<head>"
    print "<title>Delete Picture</title>"
    print "<meta http-equiv='Content-Type' content='text/html; charset=iso-8859-1'>"
    print "<link rel='stylesheet' type='text/css' href='style.css'>"
    print "<script type='text/javascript'></script>"
    print "</head>"
    print "<body>"
    print "<h2>Delete Picture</h2>"
    print "<br/>"
    print "<br/>"
    print "<div style='margin-left:auto;margin-right:auto;text-align:center;width:500px;height:150px;border:1px solid black;'>"
    print "<br/>"
    print "<br/>"
    print "<form action = 'delete.cgi' id='deleteSome' method = 'POST' enctype='multipart/form-data'>"
    print "Are you sure? You want to delete picture [" + picname + "]?"
    print "<br/>"
    print "<br/>"
    print "<br/>"
    print "<input type='hidden' id='deletepic' name = 'deletepicture' value = '" + picname + "'></input>"
    print "<input type='submit' value='Delete'></input>"
    print "<input type='button' value='Cancel' onclick='goBack()'></input>"
    print "</form>"
    print "</div>"
    print "</body>"
    print "<script>"
    print "function goBack() {"
    print "window.history.back();"
    print "}"
    print "</script>"
    
    
    print "</html>"

else:

    delete_file('deletepicture')

    print "<!DOCTYPE html>"
    print "<html>"
    print "<head>"
    print "<title>Delete Picture</title>"
    print "<meta http-equiv='refresh' content='0;gallery.cgi'/>"
    print "</head>"
    print "</html>"
