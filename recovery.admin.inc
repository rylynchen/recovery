<?php

/**
 * Administration page for config or recovery.
 */
function recovery_setting() {
	$settings = recovery_settings();
	$form = array();
	$form['mail'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('邮箱找回密码'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
    '#tree'					=> TRUE,
	);
	$form['mail']['expiry'] = array(
		'#type' => 'textfield',
		'#title' => t('有效期'),
		'#description' => t('必须为大于0的整数，单位：小时'),
		'#default_value' => $settings->mail_expiry,
	);
	$form['mail']['token'] = array(
		'#markup' => _settings_tokens_html('mail'),
	);
	$form['mail']['title'] = array(
		'#type' => 'textfield',
		'#title' => t('邮件标题'),
		'#default_value' => $settings->mail_title,
	);
	$form['mail']['body'] = array(
		'#type' => 'textarea',
		'#title' => t('邮件内容'),
		'#default_value' => $settings->mail_body,
	);
	$form['mobile'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('手机找回密码'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
    '#tree'					=> TRUE,
	);
	$form['mobile']['expiry'] = array(
		'#type' => 'textfield',
		'#title' => t('有效期'),
		'#description' => t('必须为大于0的整数，单位：小时'),
		'#default_value' => $settings->mobile_expiry,
	);
	$form['mobile']['token'] = array(
		'#markup' => _settings_tokens_html('mobile'),
	);
	$form['mobile']['body'] = array(
		'#type' => 'textarea',
		'#title' => t('短息内容'),
		'#default_value' => $settings->mobile_body,
	);
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	$form['#validate'][] = 'recovery_settings_form_validate';
	$form['#submit'][] = 'recovery_settings_form_submit';
	return $form;
}

/**
 * Form recovery_settings_form validate handler.
 */
function recovery_settings_form_validate($form, &$form_state) {
	$mail_expiry = $form_state['values']['mail']['expiry'];
	$mail_title = trim($form_state['values']['mail']['title']);
	$mail_body = trim($form_state['values']['mail']['body']);
	$mobile_expiry = $form_state['values']['mobile']['expiry'];
	$mobile_body = trim($form_state['values']['mobile']['body']);
	$tokens = recovery_settings_tokens();
	if (!(is_numeric($mail_expiry) && $mail_expiry > 0)) {
		form_set_error('mail][expiry', t('邮件有效期时间必须为大于0的整数'));
	}
	if (empty($mail_title)) {
		form_set_error('mail][title', t('邮件标题不能为空'));
	}
	if (empty($mail_body)) {
		form_set_error('mail][body', t('邮件内容不能为空'));
	}
	elseif (strpos($mail_body, $tokens['mail']['val']['link']) === FALSE) {
		form_set_error('mail][body', t('邮件内容至少要有'.$tokens['mail']['val']['link'].'参数'));
	}
	if (!(is_numeric($mobile_expiry) && $mobile_expiry > 0)) {
		form_set_error('mobile][expiry', t('短信有效期时间必须为大于0的整数'));
	}
	if (empty($mobile_body)) {
		form_set_error('mobile][body', t('短信内容不能为空'));
	}
	elseif (strpos($mobile_body, $tokens['mobile']['val']['code']) === FALSE) {
		form_set_error('mobile][body', t('短信内容至少要有'.$tokens['mobile']['val']['code'].'参数'));
	}
	if (!form_get_errors()) {
		form_set_value($form['mail']['expiry'], intval($mail_expiry), $form_state);
		form_set_value($form['mail']['title'], $mail_title, $form_state);
		form_set_value($form['mail']['body'], $mail_body, $form_state);
		form_set_value($form['mobile']['expiry'], intval($mobile_expiry), $form_state);
		form_set_value($form['mobile']['body'], $mobile_body, $form_state);
	}
}

/**
 * Form recovery_settings_form submit handler.
 */
function recovery_settings_form_submit($form, &$form_state) {
	$settings = recovery_settings();
  $settings->mail_expiry = $form_state['values']['mail']['expiry'];
  $settings->mail_title = $form_state['values']['mail']['title'];
  $settings->mail_body = $form_state['values']['mail']['body'];
  $settings->mobile_expiry = $form_state['values']['mobile']['expiry'];
  $settings->mobile_body = $form_state['values']['mobile']['body'];
  variable_set('recovery_settings', serialize($settings));
  drupal_set_message(t('Your settings has been saved.'));
}

/**
 * Form recovery_settings_form mail/mobile tokens html.
 */
function _settings_tokens_html($type) {
  $tokens = recovery_settings_tokens();
  $items = array();
  foreach ($tokens[$type]['desc'] as $key => $desc) {
  	$items[] = $tokens[$type]['val'][$key] . ' - ' . $desc;
  }
  $title = t("可使用的参数");
  $build['render'] = array(
    '#theme' => 'item_list',
    '#title' => $title,
    '#items' => $items,
    '#attributes' => array('class' => array('render-version-list'))
  );
  return drupal_render($build);
}