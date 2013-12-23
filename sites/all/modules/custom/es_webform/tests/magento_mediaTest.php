<?php
require 'PHPUnit/Autoload.php';
class es_webform_magento_mediaTest extends PHPUnit_Framework_TestCase
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
  public function testProcessMediaData() {
    $image_data['image_manager'][] = array('image_fid' =>126, 'label' => 'somelabel', 'position'=>10, 'type'=>'main');
    $image_data['image_manager'][] = array('image_fid' =>127, 'label' => 'some label 2', 'position'=>11, 'type'=>'small_image');
    $attributes = new magento_product_attributes;
    $output = $attributes->processMediaData($image_data['image_manager']);
    $this->assertTrue(is_array($output));
  }
  public function testget_marketspace_media_data_by_fid() {
    $media = new magento_media;
    $data = $media->get_marketspace_media_data_by_fid(array('image_fid' => 79, 'label' => "", 'position' => 100, 'type' => 'main'));
    
    $imageTestData->file = "Jellyfish.jpg";
    $imageTestData->label = "";
    $imageTestData->url = "http://dev.cms.communitymarketspace.com//sites/default/files/marketspace/u1/Jellyfish_0.jpg";
    $imageTestData->position = 100;
    $imageTestData->type = 'main';
    $this->assertEquals($imageTestData, $data);
  }
  public function testget_magento_media_data() {
    $media = new magento_media;
    $testData = array('image_fid' => 128, 'label' => "", 'position' => 100, 'type' => 'main');
    $media_data = $media->get_marketspace_media_data_by_fid($testData);
    //$file = file_load(128);
    //$file_url = file_create_url($file->uri); 
    //echo "hello {$file->uri} file URL : $file_url" . file_get_contents($file_url);
    $data = $media->get_magento_media_data($media_data);
    $this->assertGreaterThan(0, strlen($data['file']['content']));
    $this->assertEquals($testData['label'], $data['label']);
    $this->assertEquals($testData['position'], $data['position']);
    $this->assertEquals($testData['type'], $data['type'][0]);
  }
}