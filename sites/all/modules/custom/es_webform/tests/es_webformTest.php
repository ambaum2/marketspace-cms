<?php
require 'PHPUnit/Autoload.php';
class es_webform_Test extends PHPUnit_Framework_TestCase
{
  public $dpl_dir = '/var/www/mspacecms/';
  const DRUPAL_ROOT = '/var/www/mspacecms/';
  const REMOTE_ADDR = '162.243.110.111';
  
  public function __construct() {
    global $base_url;
    $base_url = "http://dev.cms.communitymarketspace.com/"; 
    $_SERVER['HTTP_HOST'] = basename($this->dpl_dir);
    $_SERVER['REMOTE_ADDR'] = '127.0.0.1';  
    define('DRUPAL_ROOT', $this->dpl_dir);
    set_include_path($this->dpl_dir . PATH_SEPARATOR . get_include_path());
    require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);    
  }
    private function es_webform_fields_get_all_fields() {
        $types = array_keys(es_webform_fields_field_info()); // field types defined in mymodule
        $fields = array();
        foreach (field_info_fields() as $field) {
            if (in_array($field['type'], $types)) {
                $fields = array(array(
                    'table' => _field_sql_storage_tablename($field),
                    'revision_table' => _field_sql_storage_revision_tablename($field),
                    'field_name' => $field['field_name'],
                ));
            }
        }
        return $fields;
    }
    public function testgetfieldsall() {
        $fields = $this->es_webform_fields_get_all_fields();
        print_r($fields);
        foreach($fields as $field) {
            $field_name = $field['field_name'] .'_weight';
            $data_table_name = $field['table'];
            $revision_table_name = $field['revision_table'];
            print $field_name . " datatable: " . $data_table_name . " revisiontable: " . $revision_table_name;
        }
    }
    public function testGetfields() {

        $types = array_keys(es_webform_fields_field_info()); // field types defined in mymodule
        $fields = array();
        foreach (field_info_fields() as $field) {
            if (in_array($field['type'], $types)) {
                $fields = array(
                    'table' => _field_sql_storage_tablename($field),
                    'revision_table' => _field_sql_storage_revision_tablename($field),
                    'field_name' => $field['field_name'],
                );
            }
        }
        //print_r($fields);
    }
}