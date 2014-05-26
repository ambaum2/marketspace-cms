<div class="row">
    <div class="col-md-4 col-sm-4">
        <?php foreach($ms_products_reports_list as $key => $item) : ?>
            <div class="list-group">
                <a href="products-reports?id=<?php print $item->product_id; ?>" class="list-group-item <?php $ms_active_product_id == $item->product_id ? print 'active' : '' ?>">
                    <img width="64px" class="img-responsive" src="<?php print variable_get('magento_base_media_url') . $item->thumbnail; ?>" alt="<?php print $item->name; ?>" />
                    <div class="caption">
                        <h4><?php print $item->name; ?></h4>
                    </div>
                </a>
            </div>
        <?php endforeach; ?>
    </div>
    <div class="col-md-8 col-sm-8">
        <div style='min-width: 310px; height: 400px; margin: 0 auto' id="ms-sales-chart"></div>
    </div>
</div>