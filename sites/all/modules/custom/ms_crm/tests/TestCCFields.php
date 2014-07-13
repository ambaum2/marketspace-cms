<?php
require 'PHPUnit/Autoload.php';
class TestApi extends PHPUnit_Framework_TestCase
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
    }

    /**
     * integration test for contant contact is component
     * should be pretty stable since it looks up valid
     * values for nid and cid based upon the setup ($setups)
     * keys
     */
    public function testIsConstantContactComponent() {
        $fields = new MS_Constant_Contact_Contacts_Fields();
        $setups = array('ms_crm_cc' => TRUE, 'es_webform' => FALSE, 'fieldset2' => FALSE); //query for example components
        foreach($setups as $key => $expected) {
            $component = db_query("
                select cid, nid from mspacecms.webform_component as c
                where c.type = '$key'
            ")->fetchObject();
            if(!empty($component))
                $this->assertEquals($expected, $fields->IsConstantContactComponent($component->cid, $component->nid));
        }
    }
}