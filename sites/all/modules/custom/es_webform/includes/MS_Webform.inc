<?php

class MS_Webform {
    /**
     * get nid by product attribute set id
     * @param  attribute_set_id | integer
     *    the attribute set id for a set in magento
     * @param product_type | string
     *  the product type
     * @return array
     *    an array of entity id, attribute set id, adn type_code
     * @TODO - Limit 1
     */
    public function GetWebformId($attribute_set_id, $product_type, $product_classification) {
        $query = db_select('field_data_field_market_space_settings', 'a')
            ->fields('a',array('entity_id', 'field_market_space_settings_attribute_set_id', 'field_market_space_settings_type_code', 'field_market_space_settings_product_type'))
            ->condition('field_market_space_settings_attribute_set_id', (int)$attribute_set_id)
            ->condition('field_market_space_settings_product_type', (int)$product_classification)
            ->condition('field_market_space_settings_type_code', $product_type);
        $query->leftJoin('node', 'n', 'a.entity_id = n.nid');
        $query->fields('n', array('title'));
        $query->condition('n.type', 'webform');
        $node_webform = $query->execute()->fetchAll();
        return $node_webform;
    }
}