<?php
require 'PHPUnit/Autoload.php';
class Ms_Email_UserTest extends PHPUnit_Framework_TestCase
{
    public $dpl_dir = '/var/www/mspacecms/';
    const DRUPAL_ROOT = '/var/www/mspacecms/';
    const REMOTE_ADDR = '162.243.110.111';
    public $api;
    public function __construct() {
        $_SERVER['HTTP_HOST'] = basename($this->dpl_dir);
        $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
        define('DRUPAL_ROOT', $this->dpl_dir);
        set_include_path($this->dpl_dir . PATH_SEPARATOR . get_include_path());
        require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
        drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
        $this->api = new MS_CRM_Api();
    }
    public function testSendEmail() {
//        $timestamp = REQUEST_TIME;
//        $url = url("user/reset/2/$timestamp/" . user_pass_rehash(1234, $timestamp, 'al2@thedotworldgroup.com'), array('absolute' => TRUE));
//        $email = new MS_User_Email("al2@thedotworldgroup.com");
//        $email->passwordResetUrl = $url; //"http://dev.cms.communitymarketspace.com";
//        $email->tempPassword = "1234";
//        $email->sendRegistrationEmail();
    }
    public function testUserNameExists() {
        $email = new MS_User_Email("ambaum2@gmail.com");
        print_r($email->UserNameExists());
    }
    public function testNewLocalSellerRegistration() {
        $email = new MS_User_Email("ambaum2@gmail.com");

        //$email->RegisterLocalSellerUser();
    }
}