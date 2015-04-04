#include <stdio.h>
#include "fsp.h"
#include "fcgi_stdio.h"
#include "template_def.h"

void fsp_render_tpl_page_result_entry(struct fsp_tpl_page_result_entry_ctx *tpl_ctx)
{
  const char *format = 
    "<div class=\"rbox\"> <p>%s</p> <progress id=\"res_answ%d\" value=\"%d\"" \
    " max=\"100\">%d Votes (%d%%)</progress> <p class=\"stat\">%d Votes (%d%" \
    "%)</p> </div>";

  if(NULL == tpl_ctx)
  {
    return;
  }

  printf(format, 
    tpl_ctx->entry_name, 
    tpl_ctx->entry_id, 
    tpl_ctx->entry_per, 
    tpl_ctx->entry_value, 
    tpl_ctx->entry_per, 
    tpl_ctx->entry_value, 
    tpl_ctx->entry_per
  );
}

