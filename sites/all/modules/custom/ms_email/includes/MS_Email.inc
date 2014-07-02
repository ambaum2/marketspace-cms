<?php
require 'vendor/autoload.php';
use Mailgun\Mailgun;
/**
 * Class MS_Email
 * @todo these emails need an interface
 */
class MS_Mailer {
    public $mailClient;
    public $domain;
    public $email;
    public function __construct() {
        $this->mailClient = new Mailgun('key-5iyz97dnib3u2-f0xsxap9li05ugiwe9');
        $this->domain = "smtp.communitymarketspace.com ";
    }

    /**
     * @param MS_Mail $email
     */
    public function send(MS_Mail $email) {
        $result = $this->mailClient->sendMessage($this->domain, array(
            'from'    => $email->from,
            'to'      => $email->to,
            'cc'      => $email->cc,
            'bcc'     => $email->bcc,
            'subject' => $email->subject,
            //'text'    => $email->text,
            'html'    => $email->html
        ));
    }
}