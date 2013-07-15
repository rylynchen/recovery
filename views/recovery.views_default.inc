<?php

/**
 * Implements hook_views_default_views().
 */
function recovery_views_default_views() {
	$view = new view();
	$view->name = 'recovery';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'recovery';
	$view->human_name = 'Recovery';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'none';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'rid' => 'rid',
	);
	$handler->display->display_options['style_options']['default'] = '-1';
	$handler->display->display_options['style_options']['info'] = array(
	  'rid' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	);
	/* Field: Recovery: Recovery ID */
	$handler->display->display_options['fields']['rid']['id'] = 'rid';
	$handler->display->display_options['fields']['rid']['table'] = 'recovery';
	$handler->display->display_options['fields']['rid']['field'] = 'rid';
	/* Field: Recovery: Uid */
	$handler->display->display_options['fields']['uid']['id'] = 'uid';
	$handler->display->display_options['fields']['uid']['table'] = 'recovery';
	$handler->display->display_options['fields']['uid']['field'] = 'uid';
	/* Field: Recovery: Type */
	$handler->display->display_options['fields']['type']['id'] = 'type';
	$handler->display->display_options['fields']['type']['table'] = 'recovery';
	$handler->display->display_options['fields']['type']['field'] = 'type';
	/* Field: Recovery: Code */
	$handler->display->display_options['fields']['code']['id'] = 'code';
	$handler->display->display_options['fields']['code']['table'] = 'recovery';
	$handler->display->display_options['fields']['code']['field'] = 'code';
	/* Field: Recovery: Created */
	$handler->display->display_options['fields']['created']['id'] = 'created';
	$handler->display->display_options['fields']['created']['table'] = 'recovery';
	$handler->display->display_options['fields']['created']['field'] = 'created';
	/* Field: Recovery: Expiry */
	$handler->display->display_options['fields']['expiry']['id'] = 'expiry';
	$handler->display->display_options['fields']['expiry']['table'] = 'recovery';
	$handler->display->display_options['fields']['expiry']['field'] = 'expiry';
	/* Field: Recovery: Status */
	$handler->display->display_options['fields']['status']['id'] = 'status';
	$handler->display->display_options['fields']['status']['table'] = 'recovery';
	$handler->display->display_options['fields']['status']['field'] = 'status';
	/* Field: Recovery: Operations links */
	$handler->display->display_options['fields']['operations']['id'] = 'operations';
	$handler->display->display_options['fields']['operations']['table'] = 'recovery';
	$handler->display->display_options['fields']['operations']['field'] = 'operations';
	$handler->display->display_options['fields']['operations']['label'] = 'Operations';
	/* Sort criterion: Recovery: Recovery ID */
	$handler->display->display_options['sorts']['rid']['id'] = 'rid';
	$handler->display->display_options['sorts']['rid']['table'] = 'recovery';
	$handler->display->display_options['sorts']['rid']['field'] = 'rid';
	$handler->display->display_options['sorts']['rid']['order'] = 'DESC';
	$translatables['recovery'] = array(
	  t('Master'),
	  t('more'),
	  t('Apply'),
	  t('Reset'),
	  t('Sort by'),
	  t('Asc'),
	  t('Desc'),
	  t('Items per page'),
	  t('- All -'),
	  t('Offset'),
	  t('« first'),
	  t('‹ previous'),
	  t('next ›'),
	  t('last »'),
	  t('Recovery ID'),
	  t('.'),
	  t(','),
	  t('Uid'),
	  t('Type'),
	  t('Code'),
	  t('Created'),
	  t('Expiry'),
	  t('Status'),
	  t('Operations'),
	);
  $views[$view->name] = $view;
  return $views;
}