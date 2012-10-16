<?php

/**
 * @file devudo_hosting.forms.inc
 */

define('SECRET_PASSWORD', 'drupaltogo');

/**
 * Implements hook_form_alter() for hosting_signup_form.
 */
function devudo_hosting_form_hosting_signup_form_alter(&$form, &$form_state) {
  global $user;
  drupal_set_title(t('Sign Up'));

  // Detect secret password
  if (!isset($_SESSION['secret_password']) || $_SESSION['secret_password'] != SECRET_PASSWORD){
    unset($form['site']);
    unset($form['client']);
    unset($form['#submit']);
    unset($form['#validate']);

    $form['password'] = array(
      '#title' => "What's the Password?",
      '#type' => 'password',
    );

    $form['#submit'][] = 'devudo_hosting_signup_password_submit';

    $form['submit']['#value'] = 'Open Sesame';
    $form['submit']['#weight'] = 10;
    return;
  }

  // Get Client (only if user is one already.)
  $client = is_array($user->clients)? node_load(key($user->clients)): NULL;
  //dsm($client);

  // Domain name (a.k.a. Title field)
  $form['site']['title']['#field_prefix'] = 'http://';
  
  // @TODO: Allow client domain name.
  $form['site']['title']['#field_suffix'] = '.devudo.com';
  $form['site']['title']['#size'] = 16;
  $form['site']['title']['#element_validate'] = array('devudo_hosting_signup_form_domain_validate');

  // Platform
  // @TODO: Limit based on devudo-provided platforms and the user's existing sites.
  


}

function devudo_hosting_signup_password_submit($form, &$form_state) {
  if (trim($form_state['values']['password']) != SECRET_PASSWORD){
    drupal_set_message(t('WRONG! Sorry!'), 'error');
    drupal_set_message('<img src="/sites/all/modules/devudo_hosting/wrong.jpg">', 'error');
  } else {
    $_SESSION['secret_password'] = SECRET_PASSWORD;
  }
}