# Webserver

## What features it has?

- GET a static object
    - If a requested object can be found, your server has to return "200 OK" status as well as the proper headers and content for the browser to render the document correctly.
    - If a requested object does not exist, your server has to return a "403 FORBIDDEN" status and provide error messages.
    - If a requested object is inaccessible, your server has to return a "404 NOT FOUND" status and provide error messages.
    - If a requested object is an accessible directory, please follow the descriptions in the next section.
    - If a requested URL contains a question mark (?), you can simply ignore the question mark and all texts after the mark.

- GET a directory
    - If a requested object is a directory, your program have to check whether the requested directory has a slash (/) at the end of the URL.
    - If the URL does not have a slash suffix, you have to respond a "301 MOVE PERMANENTLY" status and a new location with a slash appended to the original requested URL.
    - If a slash is already given in the URL, your server has to search for a default index file, e.g., index.html, in the directory.
    - If there is a readable index.html file, you can simply send the content of index.html back to the browser.
    - If there is a unreadable index.html file, you can simply send "403 FORBIDDEN" status to the browser.
    - If there is not an index.html file, but the directory is readable, you have to list the files and directories found in the requested directory. - The list can be either a plain-text document or a html document.
    - It would be a **plus** if your response is a html document with hyperlinks point to the location of files and directories in the requested directory.
    - If there is not an index.html file, and the directory is not readable, you have to send "404 NOT FOUND" status to the browser.

- Execute CGI programs
    - Implement CGI execution using GET requests: Environment variable - REQUEST_METHOD=GET
    - If a question mark (?) is used in the URL, add the content after (?) into a environment variable QUERY_STRING.
    - Execute the sample CGI provided by this homework. You will have to setup a pipe to forward outputs from a CGI script to the browser.
    - Implement CGI execution using POST requests: Environment variable - REQUEST_METHOD=POST. You will also have to setup a pipe to forward inputs from the browser to the CGI script.
    - **HTTP_\***

## etc

如果程式碼無法順利完成上述功能，請聯絡我 jackkuo@jackkuo.org
