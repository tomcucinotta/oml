#include "oml_scope.h"

#include <stdlib.h>

__thread oml_scope_ctx *__p_curr_ctx = NULL;

oml_scope_ctx __construct(oml_scope_ctx *p_ctx, const char *func_name) {
	p_ctx->func_name = func_name;
	p_ctx->i = 0;
	p_ctx->p_head = NULL;
	p_ctx->p_next = __p_curr_ctx;
	__p_curr_ctx = p_ctx;
	return *p_ctx;
}

void __destroy(oml_scope_ctx *p_ctx) {
	printf("__destroy: curr_ctx->func_name=%s\n",
	       __p_curr_ctx ? __p_curr_ctx->func_name : "None");
	oml_destr_node *p_node = p_ctx->p_head;
	while (p_node != NULL) {
		oml_destr_node *tmp = p_node;
		p_node->p_destructor(p_node->p_data);
		p_node = p_node->p_next;
		printf("Freeing node at %p\n", tmp);
		free(tmp);
	}
}

void __return(oml_scope_ctx *p_ctx, const char *func_name) {
	while ( __p_curr_ctx != NULL && __p_curr_ctx->func_name == func_name ) {
		__destroy(__p_curr_ctx);
		__p_curr_ctx = __p_curr_ctx->p_next;
	}
}

void __oml_scope_add(oml_scope_ctx *p_ctx, p_oml_destructor p_f, void *p_data) {
	oml_destr_node *_p_node = (oml_destr_node *) malloc(sizeof(oml_destr_node));
	printf("Allocated node at %p\n", _p_node);
	_p_node->p_next = p_ctx->p_head;
	_p_node->p_destructor = p_f;
	_p_node->p_data = p_data;
	p_ctx->p_head = _p_node;
}
