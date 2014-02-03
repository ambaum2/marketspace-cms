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
  public function testRenderMediaEditInterface() {
    //$magento_media = new magento_media;
    $product_id = 18;
    $images = $this->getImagesTestData();
    $current_fieldset = "someFieldSet";
    //tile all images with delete and edit ajax buttons
    $form[$current_fieldset]['image_manager'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
      '#attributes' => array('class' => array('row')),
    );
    //print_r($images);
    $image_count = 0;
    //print_r($images);
    $rowNumber = 0;
    foreach($images as $key=>$image) {
      if($key % 3 == 0) {
        $rowNumber = ($key / 3);
        $form[$current_fieldset]['image_manager'][$rowNumber] = array(
          '#type' => 'container',
          '#tree' => TRUE,
          '#attributes' => array('class' => array('row')),
        );       
      }
      (isset($image->types[0])) ? $imageType = $image->types[0]  : $imageType = "thumbnail"; 
      if(isset($image->types[0])) {
      $this->assertTrue(isset($imageType));
      } else {
         $this->assertEquals("thumbnail" , $imageType);
      }
      switch ($imageType) {
        case 'image':
          $weight = 1;
          break;
        case 'small_image':
          $weight = 2;
          break;
        default:
          $weight = $key + 1;
          break;
      }
      $form[$current_fieldset]['image_manager'][$rowNumber][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('col-xs-6 col-sm-6 col-md-4')),
        '#prefix' => '<div id="image-container-child">',
        '#suffix' => '</div>',
        '#weight' => $weight,
      );
      
      //$magento_media->render_image_manager_element($form, $key, $image, $current_fieldset, true);
      $image_count = $key;
    }
    for($i = 0; $i < $rowNumber; $i++) {
      $this->assertEquals(6, count($form[$current_fieldset]['image_manager'][$i])); //three elements + 3 attributes
    }
    $this->assertEquals(6, $image_count);
  }
  public function testgetMediaWeight($image, $key) {
    $productMedia = new productMedia;
    $key = 3;
    $this->assertEquals(2, $productMedia->getMediaWeight($this->getImageObjectTestData('small_image'), $key));
    $this->assertEquals(1, $productMedia->getMediaWeight($this->getImageObjectTestData('image'), $key));
    $this->assertEquals(4, $productMedia->getMediaWeight($this->getImageObjectTestData(), $key)); //3 + 1 = 4
  }
  public function testcreateMediaImageFormFields() {
    $productMedia = new productMedia;
    $images = $this->getImagesTestData();
    $values = null;
    $form["somefieldset"]['image_manager'] = array();
    foreach($images as $key=>$image) {
      $productMedia->createMediaImageFormFields($form[$current_fieldset]['image_manager'][$key], $image);
    }
    $this->assertEquals(7, count($form[$current_fieldset]['image_manager']));
  }
	public function testMediaRenderImage() {
		$component = array ( '#type' => 'fieldset', '#prefix' => '', '#suffix' => '', 'image_fid' => 12 );	
		$options = 		array(
			'label' => 'Main Image',
			'type' => 'main'
		);
		$componentInfo['image_fid'] = 12;
		$productMedia = new productMedia;
		//$productMedia->es_media_render_image_form_fields($component, $options, $componentInfo);
		//var_export($component);
		//$this->assertTrue(is_array($component));
		
	}
  /**
   * determine whether data is for add/delete, 
   * an update or no action
   * Add/delete: if an image_fid has been entered
   * then old image is to be removed and new one 
   * created for this product
   * 
   * update if the label has changed then an update 
   * needs to be done
   * 
   * otherwise do nothing
   */
  public function testProcessMediaEdit() {
    $input = $this->getProductEditTestData();
    $image_add_fields = array('file', 'url', 'type', 'position', 'label');
    $magento_media = new magento_media;
    $actions_total = array('updates' => 0, 'add_deletes' => 0, 'nothing' => 0);
    foreach($input['image_manager'] as $key => $value) {
      foreach($value as $k => $val) {
        if($val['image_fid'] > 0) {
          //this is an add/delete
          //delete first wrap in a try catch
          //$magento_media->remove_media_by_product_id($product_id, $val['current_image']);
          //add new image now that the other is gone
          $create_image_data = $magento_media->get_marketspace_media_data_by_fid($val);
          foreach($image_add_fields as $field) {
            $this->assertTrue(isset($create_image_data->$field)); //where all fields at least set
          }
          $create_image_magento_data = $magento_media->get_magento_media_data($create_image_data); //tested in testget_magento_media_data   
          //$magento_media->create_media($product_id, $create_image_magento_data);
          $actions_total['add_deletes'] = $actions_total['add_deletes'] + 1;
        } elseif($val['label'] != $val['original_label']) {
          //this is an update
          $data = array('label' => $val['label']);
          $file_name = $val['file'];
          //$return = $magento_media->update_media_by_file_name_and_product_id($product_id, $file_name, $data);
          $actions_total['updates'] = $actions_total['updates'] + 1;
        } else {
          //do nothing
          $actions_total['nothing'] = $actions_total['nothing'] + 1;
        }
      }
    }
    $this->assertEquals(2, $actions_total['add_deletes']);
    $this->assertEquals(1, $actions_total['updates']);
    $this->assertEquals(4, $actions_total['nothing']);
  }

	public function testtoColorCode($initial){
	  $checksum = md5($initial);
	  print_r( array(
	    "R" => hexdec(substr($checksum, 0, 2)),
	    "G" => hexdec(substr($checksum, 2, 2)),
	    "B" => hexdec(substr($checksum, 4, 2))
	  ));
	}  
  /** 
   * MAGENTO Media
   */
   
/*  
  public function testupdate_media_by_file_name_and_product_id() {
    $magento_media = new magento_media;
    $product_id = 7;
    $file_name = "/m/e/message-24-ok.png.png";
    $data = array("label" => "Alan Rand " . rand());
    $return = $magento_media->update_media_by_file_name_and_product_id($product_id, $file_name, $data);
    $this->assertEquals(1, $return);
  }
	public function testAlterWebformJs() {
		$form_id = "webform_client_form_11";
		$this->assertTrue(is_numeric(strpos(str_replace("_", " ", $form_id), 'webform')));
	}
  public function testGetUserImages() {
    $query = db_select('file_managed', 'f');  
    $query->condition('f.uid', 1, '=')
      ->condition('f.filename', '%%', 'LIKE')
      ->fields('f', array('fid', 'filename', 'uri'))
      ->range(0, 10);
    $result = $query->execute();
    $json_results = array();
    //print_r($result->fetchAll());
    foreach($result as $key => $value) {
      $json_results[$value->fid] = "<img width='50' height='50' src='" . file_create_url($value->uri) . "' />" . $value->filename;  
    }
    $this->assertTrue(is_array($json_results));
    $this->assertTrue(count($json_results) > 0);
  }
  public function testProcessMediaData() {
    $image_data['image_manager']['main-image'] = array('image_fid' =>126, 'label' => 'somelabel', 'position'=>10, 'type'=>'main');
    $image_data['image_manager']['label-image'] = array('image_fid' =>127, 'label' => 'some label 2', 'position'=>11, 'type'=>'small_image');
    $attributes = new magento_product_attributes;
    //print_r($image_data);
    $output = $attributes->processMediaData($image_data['image_manager']);
    $this->assertTrue(is_array($output));
		$this->assertTrue(is_array($output[0]['types'])); //image types should be an array - you can have one image serve multiple purposes
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
    $this->assertEquals($testData['type'], $data['types'][0]);
  }
 */
  public function getImageObjectTestData($type) {
    switch ($type) {
      case 'small_image':
         $object =        (object)array(
           'file' => '/J/e/Jellyfish.jpg.jpg',
           'label' => 'asdfda',
           'position' => '2',
           'exclude' => '0',
           'url' => 'http://communitymarketspace.com/media/catalog/product/J/e/Jellyfish.jpg.jpg',
           'types' =>
          array (
            0 => 'small_image',
          ),
        );
        break;
      case 'image': 
        $object =   (object)array(
         'file' => '/c/o/cocktail_orange_sm.jpg_1.jpg',
         'label' => 'asdfa',
         'position' => '1',
         'exclude' => '0',
         'url' => 'http://communitymarketspace.com/media/catalog/product/c/o/cocktail_orange_sm.jpg_1.jpg',
         'types' =>
        array (
          0 => 'image',
        ),
      );
      break;
      default:
        $object =   (object)array(
           'file' => '/H/y/Hydrangeas.jpg.jpg',
           'label' => 'asdf',
           'position' => '4',
           'exclude' => '0',
           'url' => 'http://communitymarketspace.com/media/catalog/product/H/y/Hydrangeas.jpg.jpg',
           'types' =>
          array (
          ),
        );       
        break;
    }
    return $object;
  }

  public function getProductEditTestData() {
    return array( 'product_id' => '18', 'product_edit_node_id' => '11', 'name' => 'My Image Product Test 1/7 10:43 am', 'categories' => array ( 37 => '1', 36 => NULL, 42 => NULL, 38 => NULL, 39 => NULL, 40 => NULL, 41 => NULL, ), 'status' => '1', 'price' => '3.0000', 'special_price' => '3.0000', 'special_from_date' => '2014-06-01 00:00:00', 'special_to_date' => '2014-01-22 00:00:00', 'tax_class_id' => '2', 'image_manager' => array ( 0 => array ( 0 => array ( 'image_fid' => '42', 'label' => 'asdfa', 'position' => '1', 'type' => 'image', 'current_image' => '/c/o/cocktail_orange_sm.jpg_1.jpg', 'original_label' => 'asdfa', ), 2 => array ( 'image_fid' => '', 'label' => 'updated label', 'position' => '3', 'type' => 'small_image', 'current_image' => '/J/e/Jellyfish.jpg.jpg', 'original_label' => 'asdfda', ), 1 => array ( 'image_fid' => '', 'label' => 'asdfdsaf', 'position' => '2', 'type' => 'thumbnail', 'current_image' => '/s/t/stock-photo-group-of-a-young-women-in-the-restaurant-96961214.jpg.jpg', 'original_label' => 'asdfdsaf', ), ), 1 => array ( 3 => array ( 'image_fid' => '', 'label' => 'asdf', 'position' => '4', 'type' => 'none', 'current_image' => '/H/y/Hydrangeas.jpg.jpg', 'original_label' => 'asdf', ), 4 => array ( 'image_fid' => '38', 'label' => 'asdfdsaf', 'position' => '5', 'type' => 'none', 'current_image' => '/m/e/message-24-ok.png_1.png', 'original_label' => 'asdfdsaf', ), 5 => array ( 'image_fid' => '', 'label' => 'asdfsda', 'position' => '6', 'type' => 'none', 'current_image' => '/e/-/e-marketplace_icon_1.png.png', 'original_label' => 'asdfsda', ), ), 2 => array ( 6 => array ( 'image_fid' => '', 'label' => 'asdfdsaf', 'position' => '7', 'type' => 'none', 'current_image' => '/s/t/stock-photo-young-family-mother-father-and-daughters-is-eating-hamburger-or-fast-food-at-home-114522649.jpg.jpg', 'original_label' => 'asdfdsaf', ), ), ), 'short_description' => array ( 'value' => '

asdfasdfsdafsdafdas
', 'format' => 'content_admin', ), 'description' => array ( 'value' => '

asdfsdafds
', 'format' => 'content_admin', ), 'qty' => '300.0000', 'is_in_stock' => '1', 'seller_terms_refund' => array ( 'value' => '

adfafdsafdfsdaf
', 'format' => 'content_admin', ), 'communitymarket_terms' => array ( 'value' => '

asdfdasfdafadsfsa
', 'format' => 'content_admin', ), 'op' => 'Submit', 'form_build_id' => 'form-_MdDBQHYIkMAgpkA_sSXHJJtiUY8chkBQEuVKyBhXwc', 'form_token' => '-VRayt9NSzkcRYIyYgCZw5GnStsm_sdnJzCWmDdtQgQ', 'form_id' => 'product_edit_form', );  
}
  public function getImagesTestData() {
    return array (
  0 =>
  (object)array(
     'file' => '/c/o/cocktail_orange_sm.jpg_1.jpg',
     'label' => 'asdfa',
     'position' => '1',
     'exclude' => '0',
     'url' => 'http://communitymarketspace.com/media/catalog/product/c/o/cocktail_orange_sm.jpg_1.jpg',
     'types' =>
    array (
      0 => 'image',
    ),
  ),
  1 =>
  (object)array(
     'file' => '/J/e/Jellyfish.jpg.jpg',
     'label' => 'asdfda',
     'position' => '2',
     'exclude' => '0',
     'url' => 'http://communitymarketspace.com/media/catalog/product/J/e/Jellyfish.jpg.jpg',
     'types' =>
    array (
      0 => 'small_image',
    ),
  ),
  2 =>
  (object)array(
     'file' => '/s/t/stock-photo-group-of-a-young-women-in-the-restaurant-96961214.jpg.jpg',
     'label' => 'asdfdsaf',
     'position' => '3',
     'exclude' => '0',
     'url' => 'http://communitymarketspace.com/media/catalog/product/s/t/stock-photo-group-of-a-young-women-in-the-restaurant-96961214.jpg.jpg',
     'types' =>
    array (
      
    ),
  ),
  3 =>
  (object)array(
     'file' => '/H/y/Hydrangeas.jpg.jpg',
     'label' => 'asdf',
     'position' => '4',
     'exclude' => '0',
     'url' => 'http://communitymarketspace.com/media/catalog/product/H/y/Hydrangeas.jpg.jpg',
     'types' =>
    array (
    ),
  ),
  4 =>
  (object)array(
     'file' => '/m/e/message-24-ok.png_1.png',
     'label' => 'asdfdsaf',
     'position' => '5',
     'exclude' => '0',
     'url' => 'http://communitymarketspace.com/media/catalog/product/m/e/message-24-ok.png_1.png',
     'types' =>
    array (
    
    ),
  ),
  5 =>
  (object)array(
     'file' => '/e/-/e-marketplace_icon_1.png.png',
     'label' => 'asdfsda',
     'position' => '6',
     'exclude' => '0',
     'url' => 'http://communitymarketspace.com/media/catalog/product/e/-/e-marketplace_icon_1.png.png',
     'types' =>
    array (
    ),
  ),
  6 =>
  (object)array(
     'file' => '/s/t/stock-photo-young-family-mother-father-and-daughters-is-eating-hamburger-or-fast-food-at-home-114522649.jpg.jpg',
     'label' => 'asdfdsaf',
     'position' => '7',
     'exclude' => '0',
     'url' => 'http://communitymarketspace.com/media/catalog/product/s/t/stock-photo-young-family-mother-father-and-daughters-is-eating-hamburger-or-fast-food-at-home-114522649.jpg.jpg',
     'types' =>
    array (
      
    ),
  ));
  }
}