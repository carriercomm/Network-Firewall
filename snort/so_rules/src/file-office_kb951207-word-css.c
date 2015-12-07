/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>

!!! XXX DOES NOT USE BUILT-IN DETECTION FUNCTION XXX !!! 

alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT Microsoft Word malformed css remote code execution attempt"; flow:to_client,established; flowbits:isset,file.doc; content:"<STYLE"; nocase; relative; metadata:service http, policy security-ips drop; reference:url,technet.microsoft.com/en-us/security/bulletin/MS08-026; classtype:attempted-user; sid:13790; rev:1;)

 */

#include <stdio.h>

#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

#define MAX_STYLE_TAGS 25

//#define DEBUG
#ifdef DEBUG
#define DEBUG_WRAP(code) code
#else
#define DEBUG_WRAP(code)
#endif

/* declare detection functions */
int rule13790eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_client; */
static FlowFlags rule13790flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule13790option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule13790flow0
    }
};

/* flowbits:isset "file.doc"; */
static FlowBitsInfo rule13790flowbits1 =
{
    "file.doc",
    FLOWBIT_ISSET,
    0,
};

static RuleOption rule13790option1 =
{
    OPTION_TYPE_FLOWBIT,
    {
        &rule13790flowbits1
    }
};

static CursorInfo rule13790file_data1 =
{
    0, /* offset */
    CONTENT_BUF_NORMALIZED /* flags */
};

// file_data;
static RuleOption rule13790option2 =
{
#ifndef MISSINGFILEDATA
    OPTION_TYPE_FILE_DATA,
#else
    OPTION_TYPE_SET_CURSOR,
#endif
    {
        &rule13790file_data1
    }
};
                         
// content:"<STYLE", nocase; 
static ContentInfo rule13790content1 = 
{
    (uint8_t *) "<STYLE", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_NOCASE|CONTENT_RELATIVE|CONTENT_BUF_NORMALIZED, /* flags */ // XXX - need to add CONTENT_FAST_PATTERN support
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule13790option3 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule13790content1
    }
};

/* references for sid 13790 */
/* reference: cve "2008-1434"; */
static RuleReference rule13790ref1 = 
{
    "cve", /* type */
    "2008-1434" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS08-026"; */
static RuleReference rule13790ref2 =
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS08-026" /* value */
};

static RuleReference *rule13790refs[] =
{
    &rule13790ref1,
    &rule13790ref2,
    NULL
};
/* metadata for sid 13790 */
/* metadata:service http, policy security-ips drop; */
static RuleMetaData rule13790service1 = 
{
    "service http"
};


//static RuleMetaData rule13790policy1 = 
//{
//    "policy security-ips drop"
//};


static RuleMetaData *rule13790metadata[] =
{
    &rule13790service1,
//    &rule13790policy1,
    NULL
};
RuleOption *rule13790options[] =
{
    &rule13790option0,
    &rule13790option1,
    &rule13790option2,
    &rule13790option3,
    NULL
};

Rule rule13790 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "$HTTP_PORTS", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid (HARDCODED!!!) */
       13790, /* sigid */
       10, /* revision */
   
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "FILE-OFFICE Microsoft Word malformed css remote code execution attempt",     /* message */
       rule13790refs /* ptr to references */
       ,rule13790metadata
   },
   rule13790options, /* ptr to rule options */
   &rule13790eval, /* DOES NOT use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule13790eval(void *p) {
    const uint8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    const uint8_t *end_of_payload;

    int style_count, nesting, adder;

    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;

    // flow:established, to_client;
    if (checkFlow(p, rule13790options[0]->option_u.flowFlags) <= 0 )
       return RULE_NOMATCH;

    // flowbits:isset "file.doc";
    if (processFlowbits(p, rule13790options[1]->option_u.flowBit) <= 0) 
       return RULE_NOMATCH;

    //file_data;
    #ifndef MISSINGFILEDATA
    if (fileData(p, rule13790options[2]->option_u.cursor, &cursor_normal) <= 0)
        return RULE_NOMATCH;
    #else
    if (setCursor(p, rule13790options[2]->option_u.cursor, &cursor_normal) <= 0)
        return RULE_NOMATCH;
    #endif

    if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &cursor_normal, &end_of_payload) <= 0)
        return RULE_NOMATCH;

   // content:"<STYLE", nocase;
   while(contentMatch(p, rule13790options[3]->option_u.content, &cursor_normal) > 0) {
   
      // Here's where the good stuff is
      // Our pointer is right after "<style"... Find the end of the tag
      while((cursor_normal < end_of_payload) && (*cursor_normal != '>')) {
         switch(*cursor_normal) {
            case '\"':
               while((++cursor_normal < end_of_payload) && *cursor_normal != '\"');
               break;
   
            case '\'':
               while((++cursor_normal < end_of_payload) && *cursor_normal != '\'');
               break;
   
            default:
               cursor_normal++;
         }
      }
   
      cursor_normal++;
   
      // Now we should be inside the style body.  Count style tags, being sure to
      // add the number of duplicate tags.
      style_count = 0;
      nesting = 0;
      adder = 1;
      while((cursor_normal < end_of_payload) && (*cursor_normal != '<')) {
         DEBUG_WRAP(printf("%c", *cursor_normal));
   
         switch(*cursor_normal) {
            case '{':
               nesting++;
               DEBUG_WRAP(printf("\nnesting=%d\n", nesting));
               cursor_normal++;
               break;
   
            case '}':
               nesting--;
               DEBUG_WRAP(printf("\nnesting=%d\n", nesting));
               if(nesting == 0) {
                  style_count += adder;
                  DEBUG_WRAP(printf("\nstyle_count = %d\n", style_count));
                  adder = 1;
                  if(style_count >= MAX_STYLE_TAGS) 
                     return RULE_MATCH;
               } else if(nesting < 0) {
                  // malformed CSS
                  return RULE_NOMATCH;
               }
               cursor_normal++;
               break;
   
            case '/':
               if((++cursor_normal < end_of_payload) && (*cursor_normal == '*')) {
                  DEBUG_WRAP(printf("\ncomment start\n"));
                  while(++cursor_normal < end_of_payload) { // && (*cursor_normal != '*'))
                     DEBUG_WRAP(printf("%c", *cursor_normal));
                     if(*cursor_normal == '*') {
                        if((++cursor_normal < end_of_payload) && (*cursor_normal == '/')) {
                           DEBUG_WRAP(printf("\ncomment end\n"));
                           cursor_normal++;
                           break;
                        }
                     }
                  }
               }
               break;
   
            default:
               cursor_normal++;
         }
      }
   
      if(style_count >= MAX_STYLE_TAGS)
         return RULE_MATCH;
   
   } 
 
   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule13790,
    NULL
};
*/

