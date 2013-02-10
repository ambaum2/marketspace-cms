<?php
class email_mail {
    // imap server connection
    public $conn;
 
    // inbox storage and inbox message count
    private $inbox;
    private $msg_cnt;
 
    // email login credentials
    //{mail.moorecomputing.com:143/imap/ssl/novalidate-cert} negotiation failed
    //{67.152.147.13:993/novalidate-cert/ssl} robert lee auth key? AHJvYmVydC5sZWVAYWFvbWVtYmVycy5vcmcAbGFuZ2xleTk5
    //{mail.aaortho.org:143/imap/novalidate-cert/ssl}Inbox tls/ssl failure for mail.aaortho.org: ssl negotion failed
    private $server = "{webmail.aaomembers.org:143/imap/novalidate-cert}Inbox";//'{mail.aaortho.org:143/imap/validate-cert/ssl}Inbox';//143/imap}Inbox';//67.152.147.13
    private $server_url = "webmail.aaomembers.org";
    private $user   = "robert.lee@aaomembers.org";//"pos@aaomembers.org";//'bounces@aaoinfo.org';//'aaortho.org/abaum';//"ambaum2";members_info@aaomembers.org dqw50mje
    private $pass   = "langley99";//"3131008Rsp$";//'T@cho131';
    private $port   = 143; // adjust according to server settings
 
    // connect to the server and get the inbox emails
    function __construct() {
       $this->connect();
       //$this->inbox();
    }
 
    // close the server connection
    function close() {
        $this->inbox = array();
        $this->msg_cnt = 0;
 
        imap_close($this->conn);
    }
 
    // open the server connection
    // the imap_open function parameters will need to be changed for the particular server
    // these are laid out to connect to a Dreamhost IMAP server
    function connect() {
        //imap_open("mail.aaortho.org", "aaortho.org/abaum", "T@cho131");
        $this->conn = imap_open($this->server, $this->user, $this->pass);//'{'.$this->server.'/notls}'
    }
    
    function send_mail($to,$subject,$body) {
      //@TODO - use phpmailer or swiftmailer to send messages via smtp
      require_once 'swift/lib/swift_required.php';
      
      $transport = Swift_SmtpTransport::newInstance($this->server_url, 587)
        ->setUsername($this->user)
        ->setPassword($this->pass)
        ;
      $mailer = Swift_Mailer::newInstance($transport);  
      $message = Swift_Message::newInstance($subject)
        ->setFrom(array($this->user => 'Webmaster'))
        ->setTo(array($to))
        ->setBody($body . $_SERVER['SERVER_ADDR'])
        ->setReturnPath("bounces@aaoinfo.org")
        ;    
      //$result = $mailer->send($message);  
        
    }
    // move the message to a new folder
    function move($msg_index, $folder='INBOX.Processed') {
        // move on server
        imap_mail_move($this->conn, $msg_index, $folder);
        imap_expunge($this->conn);
 
        // re-read the inbox
        $this->inbox();
    }
 
    // get a specific message (1 = first email, 2 = second email, etc.)
    function get($msg_index=NULL) {
        if (count($this->inbox) <= 0) {
            return array();
        }
        elseif ( ! is_null($msg_index) && isset($this->inbox[$msg_index])) {
            return $this->inbox[$msg_index];
        }
 
        return $this->inbox[0];
    }
 
    // read the inbox
    function inbox() {
        $this->msg_cnt = imap_num_msg($this->conn);
 
        $in = array();
        for($i = 1; $i <= $this->msg_cnt; $i++) {
            $in[] = array(
                'index'     => $i,
                'header'    => imap_headerinfo($this->conn, $i),
                'body'      => imap_body($this->conn, $i),
                'structure' => imap_fetchstructure($this->conn, $i)
            );
        }
 
        $this->inbox = $in;
    }
 
}
?>