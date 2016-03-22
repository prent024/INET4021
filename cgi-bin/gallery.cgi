#!/usr/bin/python
import cgi
import Image
import os, sys
import cgitb
cgitb.enable()

print "Content-Type: text/html\n\n"

PicDIR = "Pictures"
picList = os.listdir(PicDIR)
pic = []
for i in range(len(picList)):
    size = 140,140
    img = Image.open("Pictures/" + picList[i])
    img.thumbnail(size)
    img.save(picList[i], 'JPEG')
    pic.append(img)



print "<!DOCTYPE html>"
print "<html>"
print "<head>"
print "<link rel='stylesheet' type='text/css' href='style.css'>"
print "<script type='text/javascript'></script>"
print "<title>Picture Gallery</title>"
print "<meta http-equiv='Content-Type' content='text/html; charset=iso-8859-1'>"
print "</head>"
print "<body>"
print "<h2>Picture Gallery</h2>" 
print "<div style='text-align:center;' id='il'>"
print "<form action='' method='POST' id='uploadForm' enctype='multipart/form-data'>"
print "<button type='button' onclick='history.go(0)' value = 'Refresh'>Refresh"
print "</button>"
print "<button type='button' onclick='upload()'>Upload New Pictures"
print "</button>"
print "</form>"
print "</div>"

print "<br/>"
print "<br/>"


print "<div style='text-align:center;margin-left:auto;margin-right:auto;width:700px;height:600px;border:1px solid black;'>"
print "<br/>"


for i in range(len(pic)):
    print "<div style='display:inline-block; padding: 10px'>"
    print "<form action = '' method='POST' id=\'"+str(i)+"\'' enctype='multipart/form-data'>"
    print "<div id = 'small' name = '"+picList[i] + "'>"

    print "<img id='img' src='" + picList[i] + "' onclick='showDIV(\""+picList[i]+"\")' alt='images'>"
    print "<br/>"
    
    print picList[i]
    
    print "<input type='hidden' name='Name' id='deleteItem' value='" + picList[i]+"'></input>"
    print "<br/>"
    print "<br/>"
    print "<input type='button' onclick='Delete(\""+str(i)+"\")' value = 'Delete'></input>"
    print "<input type='button' onclick='Edit(\"" +str(i) + "\")' value='Edit'></input>"
    print "</div>"
    print "</div>"
    print "</form>"


print "</div>"
print "</body>"
print "</html>"

print "<script>"
print "function Delete(i) {"
print "var a = document.getElementById(i)"
print "a.action='delete.cgi'"
print "a.submit()"
print "}"

print "function Back() {"
print "var a = document.getElementById('uploadForm')"
print "a.action = 'login.html'"
print "a.submit()"
print "}"

print "function Edit(i) {"
print "var a = document.getElementById(i)"
print "a.action = 'edit.cgi'"
print "a.submit()"
print "}"

print "function upload() {"
print "var a = document.getElementById('uploadForm')"
print "a.action = 'upload.cgi'"
print "a.submit()"
print "}" 

print "function showDIV(i) {"
print "}"

print "</script>"


