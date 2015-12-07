/*
alert tcp $EXTERNAL_NET any -> $HOME_NET [139,445] (msg:"NETBIOS Samba SMB1 chain_reply function memory corruption attempt"; flow:to_server,established; content:"|00|"; depth:1; content:"|FF|SMBs";distance:3; within:5; content:!"|FF|"; distance:28; within:1; metadata:policy security-ips drop, service netbios-ssn; reference:cve,2010-2063; reference:bugtraq,40884; classtype:attempted-admin; sid:16728; rev:1;) 
*/
/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

#include "so-util.h"

//#define DEBUG 1
#ifdef DEBUG
#define DEBUG_SO(code) code
#else
#define DEBUG_SO(code)
#endif

#ifndef READ_BIG_24
#define READ_BIG_24(p) (*(p) << 16)              \
                | (*((uint8_t *)(p) + 1) << 8) \
                | (*((uint8_t *)(p) + 2))  
#endif

/* declare detection functions */
int rule16728eval(void *p);

/* declare rule data structures */
/* flow:established, to_server; */
static FlowFlags rule16728flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_SERVER
};

static RuleOption rule16728option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule16728flow0
    }
};
// content:"|00|", depth 1; 
static ContentInfo rule16728content1 = 
{
    (uint8_t *) "|00|", /* pattern (now in snort content format) */
    1, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16728option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16728content1
    }
};
// content:"|FF|SMBs", offset 3, depth 5, relative, fast_pattern; 
static ContentInfo rule16728content2 = 
{
    (uint8_t *) "|FF|SMBs", /* pattern (now in snort content format) */
    5, /* depth */
    3, /* offset */
    CONTENT_RELATIVE|CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16728option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16728content2
    }
};
// content:"|FF|", offset 28, depth 1, relative; 
static ContentInfo rule16728content3 = 
{
    (uint8_t *) "|FF|", /* pattern (now in snort content format) */
    1, /* depth */
    28, /* offset */
    NOT_FLAG|CONTENT_RELATIVE|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16728option3 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16728content3
    }
};

/* references for sid 16728 */
/* reference: bugtraq "40884"; */
static RuleReference rule16728ref1 = 
{
    "bugtraq", /* type */
    "40884" /* value */
};

/* reference: cve "2010-2063"; */
static RuleReference rule16728ref2 = 
{
    "cve", /* type */
    "2010-2063" /* value */
};

static RuleReference *rule16728refs[] =
{
    &rule16728ref1,
    &rule16728ref2,
    NULL
};
/* metadata for sid 16728 */
/* metadata:service netbios-ssn, policy security-ips drop; */
static RuleMetaData rule16728service1 = 
{
    "service netbios-ssn"
};


static RuleMetaData rule16728policy1 = 
{
    "policy security-ips drop"
};


static RuleMetaData *rule16728metadata[] =
{
    &rule16728service1,
    &rule16728policy1,
    NULL
};

RuleOption *rule16728options[] =
{
    &rule16728option0,
    &rule16728option1,
    &rule16728option2,
    &rule16728option3,
    NULL
};

Rule rule16728 = {
   /* rule header, akin to => tcp any any -> any any */
   {
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "any", /* SRCPORT   */
   
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "[139,445]", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       16728, /* sigid */
       3, /* revision */
       "attempted-admin", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "NETBIOS Samba SMB1 chain_reply function memory corruption attempt",     /* message */
       rule16728refs /* ptr to references */
       ,rule16728metadata
   },
   rule16728options, /* ptr to rule options */
   &rule16728eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule16728eval(void *p) {
    const uint8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    // cursor_header is used to get NetBIOS Length field and to store the beginning of SMB header
    const uint8_t *cursor_header, *end_of_payload;  
    
    uint32_t NetBIOS_Length;
    uint8_t  WordCount;
    uint8_t  AndXCommand;
    uint16_t AndXOffset;
    uint16_t prev_AndXOffset = 0;

    DEBUG_SO(printf("rule10001000eval enter\n");)
        
    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;
    
    // flow:established, to_server;
    if (checkFlow(p, rule16728options[0]->option_u.flowFlags) <= 0 )
        return RULE_NOMATCH;

    // content:"|00|", depth 1;
    if (contentMatch(p, rule16728options[1]->option_u.content, &cursor_normal) > 0)
    {
        // content:"|FF|SMBs", offset 3, depth 5, relative, fast_pattern;
        if (contentMatch(p, rule16728options[2]->option_u.content, &cursor_normal) > 0)
        {
            // check if the first AndX request is NOT 0xFF (No further commands)
            // content:"|FF|", offset 28, depth 1, relative;
#ifndef MISSING_NOT_FLAG_FIX
            if (contentMatch(p, rule16728options[3]->option_u.content, &cursor_normal) > 0)
#else
            if (!(contentMatch(p, rule16728options[3]->option_u.content, &cursor_normal) > 0))
#endif
            {
                if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &cursor_header, &end_of_payload) <= 0)
                    return RULE_NOMATCH;

                cursor_header += 1;
                NetBIOS_Length = READ_BIG_24(cursor_header);  // 3 bytes field
                cursor_header += 3;  // pointer of the beginning of SMB header
                
                DEBUG_SO(printf("NetBIOS_Length [0x%08x]\n", NetBIOS_Length);)

                cursor_normal += 27; // pointer of the first AndX request
                            
                while (cursor_normal + 5 <= end_of_payload)
                {                       
                    WordCount   = *cursor_normal++;
                    AndXCommand = *cursor_normal;

                    if (AndXCommand == 0xFF)
                        return RULE_NOMATCH;
                    
                    cursor_normal += 2;      // jump over AndXCommand and AndXreserved fields
                    AndXOffset = read_little_16(cursor_normal);

                    DEBUG_SO(printf("WordCount   [0x%02x]\n", WordCount);)
                    DEBUG_SO(printf("AndXCommand [0x%02x]\n", AndXCommand);)
                    DEBUG_SO(printf("AndXOffset  [0x%04x]\n", AndXOffset);)

                    // Malicious conditions
                    if ((WordCount > 0x0B) && (AndXOffset < 32 || AndXOffset > (NetBIOS_Length + 36)))
                        return RULE_MATCH;
                                            
                    // to prevent an infinate loop;
                    if (prev_AndXOffset >= AndXOffset)
                        return RULE_NOMATCH;

                    prev_AndXOffset = AndXOffset;                    
                    cursor_normal   = cursor_header + AndXOffset;  // offset from the beginning of SMB header
                }
            }
        }
    }

    return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule16728,
    NULL
};
*/
