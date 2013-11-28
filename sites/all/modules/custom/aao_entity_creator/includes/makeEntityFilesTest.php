<?php


require 'vendor/autoload.php';
require 'makeEntityFiles.php';

class makeEntityFilesTest extends PHPUnit_Framework_TestCase
{
    //test for mysql data types against the function
  public function testgetViewFieldHandlerByDbFieldType()
  {
      $test_types = array(
        'int(10) unsigned' => 'views_handler_field_numeric',
        'varchar(60)' => 'views_handler_field', 
        'varchar(254)' => 'views_handler_field',
        'tinyint(4)' => 'views_handler_field_numeric',
        'longblob' => 'views_handler_field',
        'int(11)' => 'views_handler_field_numeric',
        'text' => 'views_handler_field',   
      );
      
      foreach($test_types as $key => $value) {
        $data['Type'] = $key;
        $handler = getViewFieldHandlerByDbFieldType($data);
        $this->assertEquals($handler, $value);
      }
  }
  //test cleanMysqlTypeText
  public function testcleanMysqlTypeText() {
    $test_types = array(
      'int(10) unsigned' => 'int',
      'varchar(60)' => 'varchar', 
      'varchar(254)' => 'varchar',
      'tinyint(4)' => 'tinyint',
      'longblob' => 'longblob',
      'int(11)' => 'int',
      'text' => 'text',  
    );
    foreach($test_types as $key => $value) {
      $cleaned_string = cleanMysqlTypeText($key);
      $this->assertEquals($cleaned_string, $value);
    }
  }

}
?>
