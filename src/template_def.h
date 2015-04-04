#pragma once

#define FSP_TPL_DOC_BEG \
    "<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"utf-8\"> <met" \
    "a name=\"viewport\" content=\"inital-scale=1.0, width=device-width\"> <" \
    "link rel=\"stylesheet\" href=\"css/main.css\"> <link rel=\"stylesheet\"" \
    " href=\"css/mobi.css\" media=\"all and (max-width: 799px)\"> <title>Fas" \
    "terPoll</title> </head> <body>"

#define FSP_TPL_DOC_END \
    "<script src=\"js/main.js\" defer></script> </body> </html>"

#define FSP_TPL_HEADER \
    "<header id=\"head\"> <div class=\"page\"> <h1>FasterPoll</h1> <nav id=\" \
    ""tnav\"> <a href=\"index.html\">Home</a> <a href=\"vote.html\">Vote</a>" \
    " <a href=\"result.html\">Result</a> </nav> </div> </header>"

#define FSP_TPL_MAIN_BEG \
    "<main role=\"main\" id=\"main\"> <div class=\"page\">"

#define FSP_TPL_MAIN_END \
    "</div> </main>"

#define FSP_TPL_FOOTER \
    "<footer id=\"foot\"> <p class=\"page copy\">&copy; 2015 FasterPoll</p> " \
    "<p class=\"page\"><a href=\"#\">View source on Github</a></p> </footer>"

#define FSP_TPL_PAGE_HOME \
    "<h2>Create new poll</h2> <form class=\"form cbox form--poll\" action=\"" \
    "\" method=\"post\" data-answ=\"4\"> <div class=\"view\"> <div class=\"f" \
    "box\"> <label for=\"poll_title\">Title:</label> <textarea id=\"poll_tit" \
    "le\" placeholder=\"Enter title here ...\"></textarea> </div> <div class" \
    "=\"fbox answ\"> <label for=\"poll_answ0\">Answer #1:</label> <input typ" \
    "e=\"text\" class=\"text\" id=\"poll_answ0\" placeholder=\"Enter option " \
    "here ...\"> </div> <div class=\"fbox answ\"> <label for=\"poll_answ1\">" \
    "Answer #2:</label> <input type=\"text\" class=\"text\" id=\"poll_answ1\" \
    "" placeholder=\"Enter option here ...\"> </div> <div class=\"fbox answ\" \
    ""> <label for=\"poll_answ2\">Answer #3:</label> <input type=\"text\" cl" \
    "ass=\"text\" id=\"poll_answ2\" placeholder=\"Enter option here ...\"> <" \
    "/div> <div class=\"fbox answ\"> <label for=\"poll_answ3\">Answer #4:</l" \
    "abel> <input type=\"text\" class=\"text\" id=\"poll_answ3\" placeholder" \
    "=\"Enter option here ...\"> </div> <div class=\"more\"><!-- additional " \
    "answers --></div> <div class=\"fbox\"> <input type=\"button\" class=\"b" \
    "tn btn--semi add\" value=\"Add field\"> <p class=\"desc\">Up to 100 fie" \
    "lds per poll</p> </div> <div class=\"fbox\"> <label> <input type=\"chec" \
    "kbox\"> Allow multiple choice </label> <label> <input type=\"checkbox\"" \
    "> Allow duplicate votes (from same IP-Address) </label> </div> </div> <" \
    "div class=\"ftrl\"> <input type=\"submit\" class=\"btn btn--main\" valu" \
    "e=\"Create\"> </div> </form>"

#define FSP_TPL_PAGE_RESULT_BEG \
    "<h2>Poll result</h2> <div class=\"cbox view\"> <p class=\"pqst\">%s</p>"

#define FSP_TPL_PAGE_RESULT_END \
    "</div> <p class=\"pown\"><a href=\"#\">Create your own poll!</a></p>"

struct fsp_tpl_page_result_entry_ctx
{
  char* entry_name;
  int entry_value;
  int entry_per;
  int entry_id;
};

void fsp_render_tpl_page_result_entry(struct fsp_tpl_page_result_entry_ctx *tpl_ctx);
