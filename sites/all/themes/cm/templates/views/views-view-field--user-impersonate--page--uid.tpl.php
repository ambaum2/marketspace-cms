<?php
/** business logic in controller!!!! Oh my! - but seriously move to a static method or pass from the controller at some point - of course this is drupal so...
 by the way if anybody sees this - which I doubt anyone ever will because I most likely failed in my attempt to make community marketspace succeed or someone
 * won a legal battle against me somehow (not sure how i'm not under any contract. really just doing this all for jim out of the kindness of my heart for free). give me an email at ambaum2@gmail.com
 * and title it "you don't suck but you should be murdered for placing business logic inside a view ...asshole" and just for emailing me that I'm going to purposely use print and wrap
 * my code in sinble quotes just so you have to rewrite it. When I could have easily ended the code block and made things pretty - :). In fact it took me longer to write this paragrapsh
 * then to actually write that snippet below. Have a nice life
 */
$path = 'devel/switch/'. $row->users_name;
$dest = drupal_get_destination();
$token = drupal_get_token($path . '|' . '/user');

print '<a href="/devel/switch/' . urlencode($row->users_name) . '?destination=/user&token=' . $token . '">Login As ' . $row->users_name . '</a>';
