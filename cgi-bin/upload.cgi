#!/usr/bin/python
import cgi
import cgitb; cgitb.enable()
import os

UPLOAD_DIR = "Pictures"
print "Content-Type: text/html\n\n"

HTML_TEMPLATE="""
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
    <head>
    <script type="text/javascript"></script>
    <link rel="stylesheet" type="text/css" href="style.css">
    <title>Upload New Picture</title>
    <meta http-equiv="Content-Type" content = "text/html;charset=iso-8859-1">
    </head>
<body>
    <h2>Upload a New JPEG Picture</h2>
    <div style="margin-left:auto;margin-right:auto;text-align:center;width:500px;height:180px;border:1px solid black;"><br/><br/>
    <form action="" method="POST" id="uploadF" enctype="multipart/form-data"><br/>
    Title: <input name="title" id='title' type="text" maxlength="100"></input><br/><br/>
    File: <input name="file_input" type="file" accept="image/jpeg, image/jpg"></input><br/><br/>
    <input name="submit" type="submit" value="Upload"></input>
    
    <input type="button" value="Cancel" onclick="location.href='gallery.cgi'"></input>
    
    </form>
    </div>
<script>
function cancel(){
    var a = document.getElementById("uploadF");
    a.action = "gallery.cgi";
    a.submit();
}
</script>
</body>
</html>

"""

HTML_ERROR="""
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
    <head>
    <script type="text/javascript"></script>
    <link rel="stylesheet" type="text/css" href="style.css">
    <title>Upload New Picture</title>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
    </head>
<body>
    <h2>Upload a New JPEG Picture</h2>
    <div style="margin-left:auto;margin-right:auto;text-align:center;width:500px;height:180px;border:1px solid black;"> 
    
    <form action="" method="POST" id= "uploadF" enctype="multipart/form-data">
        <p style="color: red">File Title Cannot Be Empty</p>
        Title: <input name="title" id="title" type="text" maxlength="100"></input><br/><br/>
        File: <input name="file_input" id="file_input" type = "file" accept="image/jpeg, image/jpg"></input><br/><br/>
        <input type="submit" name ="submit" value="Upload"></input>
        <input type="button" value="Cancel" onclick="location.href='gallery.cgi'"></input>
    </form>
    </div>
<script>
function cancel(){
    var a = document.getElementById("uploadF");
    a.action = "gallery.cgi";
    a.submit();
}
</script>
</body>
</html>

"""    

DIRECT_BACK="""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta http-equiv="refresh" content="0; gallery.cgi"/>
</head>
</html>
"""

def save_uploaded_file(form_field, upload_dir):
    form = cgi.FieldStorage()
    if form.has_key('title') and len(form['title'].value) == 0:
        print HTML_ERROR
        return
    if not form.has_key(form_field):
        print HTML_TEMPLATE
        return
    fileitem = form[form_field]
    if not fileitem.file or len(fileitem.filename) == 0:
        return
    fout = file(os.path.join(upload_dir, form['title'].value), 'w')
    while 1:
        chunk = fileitem.file.read(1000000000)
        if not chunk: break
        fout.write(chunk)
        fout.close()
    print DIRECT_BACK    
save_uploaded_file("file_input", UPLOAD_DIR)
