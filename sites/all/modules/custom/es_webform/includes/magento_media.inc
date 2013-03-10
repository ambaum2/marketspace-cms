<?php

class magento_media extends magento {

  /*
   * create new media for a product
   */
   public function create_media($session_id, $product_id, $product_data, $file_path, $sku, $image_types = array('thumbnail','image'), $host = magento::primary_host) {
      $proxy = new SoapClient($host);
      $image = file_get_contents($file_path);
      $file_info = new finfo(FILEINFO_MIME_TYPE);
      $mime_type = $file_info->buffer($image);
      $file_content = base64_encode($image);
      $file = array(
      'content' => $file_content,
      'mime' => $mime_type,
      //'name' => $product_data['name'] . $sku,
      );
     try {    
        $result = $proxy->catalogProductAttributeMediaCreate(
          $session_id,
          $product_id,
          array('file' => $file, 'label' => 'a label', 'position' => '10', 'types' => $image_types, 'exclude' => 0)
        );
        return $result;
     } catch (exception $e) {
        throw $e;
     }
  }
    
}
  