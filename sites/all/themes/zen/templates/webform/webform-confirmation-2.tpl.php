<?php

/**
 * @file
 * Customize confirmation screen after successful submission.
 *
 * This file may be renamed "webform-confirmation-[nid].tpl.php" to target a
 * specific webform e-mail on your site. Or you can leave it
 * "webform-confirmation.tpl.php" to affect all webform confirmations on your
 * site.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $confirmation_message: The confirmation message input by the webform author.
 * - $sid: The unique submission ID of this submission.
 */
?>

<div class="webform-confirmation">
  <?php if ($confirmation_message): ?>
    <?php print $confirmation_message ?>
  <?php else: ?>
    <?php
    
      module_load_include("inc","webform", "includes/webform.submissions");
      $cid = webform_get_cid($node, 'profile_name', 15); //15 is parent id
     $submission = webform_get_submission($node->nid, $sid);
     //dpm($submission);
     $url = preg_replace("![^a-z0-9]+!i", "-", $submission->data[$cid]['value'][0]);

    ?>
    <p><?php print t('Thank you, your Venue has been created.'); ?></p>
    <a href=<?php print "'http://aaortho.net/index.php/" .  strtolower($url) . ".html'" ?>>View Your Product on Escape Locally</a>
  <?php endif; ?>
</div>

<div class="links">
  <a href="<?php print url('node/'. $node->nid) ?>"><?php print t('Go back to the form') ?></a>
</div>
