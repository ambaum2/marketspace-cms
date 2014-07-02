<?php

class MS_Mail {
    public $from;
    public $to;
    public $cc;
    public $bcc;
    public $subject;
    public $text;
    public $html;

    public function __construct($to, $cc, $bcc) {
        $this->setCc($cc);
        $this->setBcc($bcc);
        $this->to = $to;
        $this->from = 'mms@communitymarketspace.com';
    }
    /**
     * @param mixed $cc
     */
    public function setCc($cc)
    {
        if(!empty($cc) && is_array($cc)) {
            $this->cc = $cc;
        } else {
            $this->cc = array();
        }
    }

    /**
     * @param mixed $bcc
     */
    public function setBcc($bcc)
    {
        if(!empty($bcc) && is_array($bcc)) {
            $this->bcc = $bcc;
        } else {
            $this->bcc = array();
        }
    }
}