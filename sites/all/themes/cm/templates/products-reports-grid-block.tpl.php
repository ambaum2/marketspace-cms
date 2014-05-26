<div class="row">
    <div class="col-md-12 col-sm-12">
        <div class="table-responsive">
            <table class="table">
                <thead>
                <tr>
                    <td>Order #</td>
                    <td>Purchased On</td>
                    <td>Name</td>
                    <td>Customer Name</td>
                    <td>Total</td>
                    <td>Status</td>
                    <td>Details</td>
                </tr>
                </thead>
                <tbody>
                <?php foreach($ms_products_reports_list as $key => $item) : ?>
                    <tr>
                        <td><?php print $item->increment_id . ' - ' . $item->item_id; ?></td>
                        <td><?php print strftime('%m/%d/%Y %R %P', strtotime($item->updated_at));  ?></td>
                        <td><?php print $item->name; ?></td>
                        <td><?php print $item->customer_firstname . ' ' . $item->customer_lastname; ?></td>
                        <td><?php print '$' . number_format($item->total_incl_tax, 2); ?></td>
                        <td><?php print $item->status; ?></td>
                        <td>
                            <button class="btn btn-primary btn-lg" data-toggle="modal" data-target=".ms-detail-modal-<?php print $item->item_id; ?>">
                                Details
                            </button>
                            <div class="modal fade ms-detail-modal-<?php print $item->item_id; ?>" role="dialog" aria-labelledby="myLargeModalLabel" aria-hidden="true">
                                <div class="modal-dialog">
                                    <div class="modal-content well">
                                        <div class="row">
                                            <div class="col-md-12">
                                                <h3>
                                                    Order # <?php print $item->increment_id . ' - ' . $item->item_id; ?>
                                                </h3>
                                                <h4>
                                                    <?php print strftime('%m/%d/%Y %R %P', strtotime($item->updated_at)); ?>
                                                </h4>
                                                <h4>
                                                    <?php print $item->name; ?>
                                                </h4>
                                            </div>
                                        </div>
                                        <div class="row">
                                            <div class="col-md-6">
                                                <address>
                                                    <h5>Shipping Information</h5>
                                                    <?php if($item->is_virtual) : ?>
                                                        <h4>None for this item</h4>
                                                    <?php else : ?>
                                                    <ul class="nav">
                                                        <li><?php print $item->customer_firstname . ' ' . $item->customer_lastname; ?></li>
                                                        <li><?php print $item->street; ?></li>
                                                        <li><?php print $item->city . ', ' . $item->region . ' ' . $item->postcode; ?></li>
                                                        <li><?php print $item->telephone; ?></li>
                                                    </ul>
                                                    <?php endif; ?>
                                                </address>
                                            </div>
                                            <div class="col-md-6">
                                                <h5>Payment Information</h5>
                                                Method: <?php print $item->method; ?>
                                            </div>
                                        </div>
                                        <div class="row">
                                            <div class="col-md-12 table-responsive">
                                                <table class="table">
                                                    <thead>
                                                    <tr>
                                                        <td>Status</td>
                                                        <td>Quantity</td>
                                                        <td>Weight</td>
                                                        <td>Price</td>
                                                        <td>Tax</td>
                                                        <td>Total</td>
                                                    </tr>
                                                    </thead>
                                                    <tbody>
                                                        <td><?php print $item->status; ?></td>
                                                        <td><?php print $item->qty; ?></td>
                                                        <td><?php print $item->weight; ?></td>
                                                        <td><?php print '$' . number_format($item->price, 2); ?></td>
                                                        <td><?php print '$' . number_format($item->tax, 2); ?></td>
                                                        <td><?php print '$' . number_format($item->total_incl_tax, 2); ?></td>
                                                    </tbody>
                                                </table>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </td>
                    </tr>
                <?php endforeach; ?>
                </tbody>
            </table>
        </div>
    </div>
</div>