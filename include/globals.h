#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/*
 * Domain name
 */
#define DOMAIN_NAME "localhost"


/*
 * Marks end of mail content transmission
 */
#define END_DATA "\r\n.\r\n"

/*
 * Reply codes as defined in RFC-821
 *
 */
#define SYSTEM_STATUS 211
#define HELP_MESSAGE 214
#define SERVICE_READY 220
#define SERVICE_CLOSING 221
#define MAIL_ACTION_OK 250
#define USER_NOT_LOCAL_FORWARDED 251
#define START_MAIL_INPUT 354 //end with <CRLF>.<CRLF>
#define SERVICE_NA_CLOSING 421
#define FAIL_MAILBOX_UNAVAILABLE 450
#define ERROR_IN_PROCESSING 451
#define FAIL_INSUFFICIENT_STORAGE 452
#define CMD_NOT_RECOGNIZED 500
#define SYNTAX_ERROR_IN_ARGUMENTS 501
#define CMD_NOT_IMPLEMENTED 502
#define BAD_CMD_SEQUENCE 503
#define PARAMETER_NOT_IMPLEMENTED 504
#define MAILBOX_NOT_FOUND 550
#define USER_NOT_LOCAL 551
#define MAIL_ACTION_ABORT_STORAGE 552
#define MAILBOX_SYNTAX_ERROR 553
#define TRANSACTION_FAILED 554
#define END_OF_DATA 355

/*
 * Maximum lengths
 */
#define MAX_USERNAME 64
#define MAX_DOMAIN 64
#define MAX_CMD 512
#define MAX_REPLY 512
#define MAX_TEXT_LINE 1000
#define MAX_RCPTS 100

/*
 *
 * SMTP states & commands
 *
 */

#define SMTP_INIT 0
#define SMTP_HELO 1
#define SMTP_MAIL 2
#define SMTP_RCPT 3
#define SMTP_DATA 4
#define SMTP_RSET 5
#define SMTP_QUIT 6
#define SMTP_NOOP 7
#define SMTP_BAD_CMD 8
#define SMTP_SUBJ 9


/*
 * POP reply codes
 */ 

#define USER_OK 0
#define USER_ERR 1
#define PASS_OK 2
#define PASS_ERR 3
#define QUIT_AUTH_OK 4
#define QUIT_AUTH_ERR 5
#define QUIT_TRAN_OK 6
#define QUIT_TRAN_ERR 7
#define BAD_CMD_SEQ 8
#define STAT_OK 9
#define LIST_OK 10
#define LIST_ERR 11
#define RETR_OK 12
#define RETR_ERR 13
#define DELE_OK 14
#define DELE_ERR 15
#define GREETING 16
#define REPLY_OK 17
#define REPLY_ERR 18
#define CONTENT_TRANSMIT 19

/*
 * Pop Commands
 */

#define POP_USER 0
#define POP_PASS 1
#define POP_STAT 2
#define POP_LIST 3
#define POP_RETR 4
#define POP_DELE 5
#define POP_NOOP 6
#define POP_RSET 7
#define POP_QUIT 8
#define POP_BAD 9

/*
 * POP FSM states
 */

#define POP_STATE_AUTH 0
#define POP_STATE_TRAN 1
#define POP_STATE_UPDATE 2

/*
 * EditUser constants
 */ 

#define EDIT_USER_ADDU "ADDU"
#define EDIT_USER_REMU "REMU"
#define EDIT_USER_QUIT "QUIT"

#endif
