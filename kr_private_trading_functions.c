#include "kr_private_trading_functions.h"

static struct st_opt_list type_table[] = {
	[MARKET]                 =   {.name = "market"},
	[LIMIT]                  =   {.name = "limit"},
	[STOP_LOSS]              =   {.name = "stop-loss"},
	[TAKE_PROFIT]            =   {.name = "take-profit"},
	[STOP_LOSS_PROFIT]       =   {.name = "stop-loss-profit"},
	[STOP_LOSS_PROFIT_LIMIT] =   {.name = "stop-loss-profit-limit"},
	[STOP_LOSS_LIMIT]        =   {.name = "stop-loss-limit"},
	[TAKE_PROFIT_LIMIT]      =   {.name = "take-profit-limit"},
	[TRAILING_STOP]          =   {.name = "trailing-stop"},
	[TRAILING_STOP_LIMIT]    =   {.name = "trailing-stop-limit"},
	[STOP_LOSS_AND_LIMIT]    =   {.name = "stop-loss-and-limit"},
	[SETTLE_POSITION]        =   {.name = "settle-position"}
};

#undef NKEYS
#define NKEYS (uint8_t)(sizeof(type_table)/sizeof(type_table[0]))

int addOrder(
		struct kraken_api **kr_api,
		const char *type,
		const char *order,
		const char *asset,
		const char *volume, ...){

	va_list ap;
	const char *var_arg = NULL;
	va_start(ap, volume);

	const char* url_seperator   = "&";
	const char* url_pair        = "pair=";
	const char* url_type        = "type=";
	const char* url_ordertype   = "ordertype=";
	const char* url_volume      = "volume=";
	const char* url_price_1     = "price=";
	const char* url_price_2     = "price2=";
	const char* url_trading     = "trading_agreement=";
	const char* var_agreement   = "agree";

	PTRACEX("type: %s\n", type);
	PTRACEX("order: %s\n", order);
	PTRACEX("asset: %s\n", asset);
	
	/* create the temporary url for this type of api call */

	if(((*kr_api)->tmp_query_url = strdup((*kr_api)->s_uri_private)) == NULL){
		PERROR("ERROR - strdup malloc failed");
		return -1;
	}

	(*kr_api)->tmp_query_url = to_url((*kr_api)->tmp_query_url, (*kr_api)->s_uri_addorder);
	
	/* start the data string */
	if(((*kr_api)->s_data = strdup(url_pair)) == NULL){
		PERROR("ERROR on strdup");
		return -1;
	}

	(*kr_api)->s_data = to_url((*kr_api)->s_data, asset);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_type);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, type);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_ordertype);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, order);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_volume);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, volume);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_trading);
	(*kr_api)->s_data = to_url((*kr_api)->s_data, var_agreement);

	switch (key_from_string(order, type_table, NKEYS)){

		case MARKET:
			PTRACE("switch to case market");
			/* market only needs VOLUME */
			PTRACEX("MARKET URL: %s", (*kr_api)->s_data);
			break;

		case LIMIT:
			PTRACE("switch to case limit\n");
			/* limit needs 1x va_arg: limit price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("LIMIT URL: %s", (*kr_api)->s_data);
			break;

		case STOP_LOSS:
			PTRACE("switch to case stop-loss\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = stop-loss-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("STOP LOSS URL: %s", (*kr_api)->s_data);
			break;

		case TAKE_PROFIT:
			PTRACE("switch to case take-profit\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = take-profit-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("TAKE PROFIT URL: %s", (*kr_api)->s_data);
			break;

		case STOP_LOSS_PROFIT:
			PTRACE("switch to case stop-loss-profit\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = stop-loss-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			/* price_2 = take-profit-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_2);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("STOP LOSS PROFIT URL: %s", (*kr_api)->s_data);
			break;

		case STOP_LOSS_PROFIT_LIMIT:
			PTRACE("switch to case stop-loss-profit-limit\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = stop-loss-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			/* price_2 = take-profit-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_2);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("STOP LOSS PROFIT LIMIT URL: %s", (*kr_api)->s_data);
			break;

		case STOP_LOSS_LIMIT:
			PTRACE("switch to case stop-loss-limit\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = stop-loss-trigger-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			/* price_2 = triggered-limit-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_2);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("STOP LOSS LIMIT URL: %s", (*kr_api)->s_data);
			break;

		case TAKE_PROFIT_LIMIT:
			PTRACE("switch to case take-profit-limit\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = take-profit-trigger-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			/* price_2 = triggered-limit-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_2);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("TAKE PROFIT URL: %s", (*kr_api)->s_data);
			break;

		case TRAILING_STOP:
			PTRACE("switch to case trailing-stop\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = trailing stop offset */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("TRAILING STOP URL: %s", (*kr_api)->s_data);
			break;

		case TRAILING_STOP_LIMIT:
			PTRACE("switch to case trailing-stop-limit\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = trailing stop offset */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			/* price_2 = triggered-limit-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_2);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("TRAILING STOP URL: %s", (*kr_api)->s_data);
			break;

		case STOP_LOSS_AND_LIMIT:
			PTRACE("switch to case stop-loss-and-limit\n");
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			/* price_1 = stop-loss-price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_1);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			/* price_2 = limit price */
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_seperator);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, url_price_2);
			var_arg = va_arg(ap, char*);
			(*kr_api)->s_data = to_url((*kr_api)->s_data, var_arg);
			PTRACEX("STOP LOSS AND LIMIT URL: %s", (*kr_api)->s_data);
			break;

		case SETTLE_POSITION:
			PTRACE("switch to case settle-position\n");
			/* settle-position only needs VOLUME */
			PTRACEX("SETTLE POSITION URL: %s", (*kr_api)->s_data);
			break;

		case BADARG:
			PERROR("switch to case BADARG ERROR\n");
			return -1; /* <-- EXIT POINT */
			break;
	}

	/* set the appropriate optionals for this function */
	(*kr_api)->opt_table[LEVERAGE].b_flag       = TRUE;
	(*kr_api)->opt_table[OFLAGS].b_flag         = TRUE;
	(*kr_api)->opt_table[STARTTM].b_flag        = TRUE;
	(*kr_api)->opt_table[EXPIRETM].b_flag       = TRUE;
	(*kr_api)->opt_table[USERREF].b_flag        = TRUE;
	(*kr_api)->opt_table[VALIDATE].b_flag       = TRUE;
	(*kr_api)->opt_table[CLOSE_TYPE].b_flag     = TRUE;
	(*kr_api)->opt_table[CLOSE_PRICE_1].b_flag  = TRUE;
	(*kr_api)->opt_table[CLOSE_PRICE_2].b_flag  = TRUE;

	/* check if optionals are given */
	switch_opt(kr_api);

    va_end(ap);

    query_private(kr_api);

    return 0;
}


int cancelOrder(struct kraken_api **kr_api, const char *txid){

	const char* url_txid = "txid=";

	/* create the temporary url */
	(*kr_api)->tmp_query_url = to_url((*kr_api)->tmp_query_url, (*kr_api)->s_uri_private);
	(*kr_api)->tmp_query_url = to_url((*kr_api)->tmp_query_url, (*kr_api)->s_uri_cancel_order);

	/* start the data string */
	(*kr_api)->s_data = to_url((*kr_api)->s_data, url_txid);

	/* add XXBTZEUR to the string */
	(*kr_api)->s_data = to_url((*kr_api)->s_data, txid);

	query_private(kr_api);

	return 0;
}
