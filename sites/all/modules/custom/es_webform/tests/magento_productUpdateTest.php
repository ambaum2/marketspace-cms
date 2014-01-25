<?php
require 'PHPUnit/Autoload.php';
class es_magento_product_UpdateTest extends PHPUnit_Framework_TestCase
{
  public $dpl_dir = '/var/www/mspacecms/';
  const DRUPAL_ROOT = '/var/www/mspacecms/';
  const REMOTE_ADDR = '162.243.110.111';
  
  public function __construct() {
    $_SERVER['HTTP_HOST'] = basename($this->dpl_dir);
    $_SERVER['REMOTE_ADDR'] = '127.0.0.1';  
    define('DRUPAL_ROOT', $this->dpl_dir);
    set_include_path($this->dpl_dir . PATH_SEPARATOR . get_include_path());
    require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);    
  }
 
  public function testUpdateProduct() {
    $magento = new magento_product;
    $data['special_from_date'] = "06/03/2014";
    $data['special_to_date'] = "06/18/2014 00:00:00";
    $magento->updateProduct(30, $data);
  } 
}