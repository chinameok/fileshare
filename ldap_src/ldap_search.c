#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <lber.h>
#include <ldap.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <json-c/json.h>

#include <stdlib.h>

#define RCSID "$Id: ldap_search.c,v 1.4 2003/07/14 15:21:56 rory Exp $"


void  BlToSl(char str[100])
{

	int i;
	
	for(i=0;i< strlen(str);i++)
	{
		str[i] = tolower(str[i]);
	}
		
	//printf("%s\r\n",lowletter);

}



int getComputorName()
{
   LDAP *ld;
   struct json_object  *my_object,*my_array;
   const char* split = "CN=,";
   char *p;
   char*tmp;
   char *user;
   int i = 0;
   int nl ;
   int  result;
   int  auth_method = LDAP_AUTH_SIMPLE;
   int desired_version = LDAP_VERSION3;
   char *ldap_host = "WIN2K82";
   char *root_dn = "administrator@abc.com";
   char *root_pw = "123qwe!@#";

   BerElement* ber;
   LDAPMessage* msg;
   LDAPMessage* entry;

   char* base="CN=Computers,dc=abc,dc=com";
   char* filter="(objectClass=computer)";
   char* errstring;
   char* dn = NULL;
   char* attr;
   char** vals;
   

   if ((ld = ldap_init(ldap_host, LDAP_PORT)) == NULL )
    {
      perror( "ldap_init failed" );
      exit( EXIT_FAILURE );
   }

   /* set the LDAP version to be 3 */
   if (ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &desired_version) != LDAP_OPT_SUCCESS)
   {
      ldap_perror(ld, "ldap_set_option");
      exit(EXIT_FAILURE);
   }

   if (ldap_bind_s(ld, root_dn, root_pw, auth_method) != LDAP_SUCCESS ) {
      ldap_perror( ld, "ldap_bind" );
      exit( EXIT_FAILURE );
   }

   if (ldap_search_s(ld, base, LDAP_SCOPE_SUBTREE, filter, NULL, 0, &msg) != LDAP_SUCCESS) {
      ldap_perror( ld, "ldap_search_s" );
      exit(EXIT_FAILURE);
   }

    my_object = json_object_new_object();
    my_array = json_object_new_array();
   int n = 0;//find return 
   /* Iterate through the returned entries */
   for(entry = ldap_first_entry(ld, msg); entry != NULL; entry = ldap_next_entry(ld, entry)) {

      if((dn = ldap_get_dn(ld, entry)) != NULL) {


         
	//printf("Returned dn: %s\n", dn);

        tmp = dn;
        p = strtok (tmp,",");
        if(p){

        p = strtok(p,"=");  
        p = strtok(NULL, ",");
	BlToSl(p);      

	}        
        i++;
	
	
        json_object_array_add(my_array, json_object_new_int(i));
        json_object_array_put_idx(my_array, i, json_object_new_int(i));
	 struct json_object *obj = json_object_array_get_idx(my_array, i);
  
	json_object_object_add(my_object, json_object_to_json_string(obj), json_object_new_string(p));
         
	 ldap_memfree(dn);
      }
	 
     
}
    //second search
   base="OU=Domain Controllers,dc=abc,dc=com";
   filter="(objectClass=computer)";
 if (ldap_search_s(ld, base, LDAP_SCOPE_SUBTREE, filter, NULL, 0, &msg) != LDAP_SUCCESS) {
      ldap_perror( ld, "ldap_search_s" );
      exit(EXIT_FAILURE);
   }



for(entry = ldap_first_entry(ld, msg); entry != NULL; entry = ldap_next_entry(ld, entry)) {

      if((dn = ldap_get_dn(ld, entry)) != NULL) {
	//printf("Returned dn: %s\n", dn);
        tmp = dn;
        p = strtok (tmp,",");
        if(p){

        p = strtok(p,"=");  
        p = strtok(NULL, ",");  
	BlToSl(p);    
        //if (p)     
	//printf("p=%s\n",p);
	}        
        i++;
	json_object_array_add(my_array, json_object_new_int(i));
        json_object_array_put_idx(my_array, i, json_object_new_int(i));
	 struct json_object *obj = json_object_array_get_idx(my_array, i);
  
	json_object_object_add(my_object, json_object_to_json_string(obj), json_object_new_string(p));
         
	 ldap_memfree(dn);
      }


}

	//third search
     base="CN=Users,dc=abc,dc=com";
   filter="(objectClass=computer)";

 if (ldap_search_s(ld, base, LDAP_SCOPE_SUBTREE, filter, NULL, 0, &msg) != LDAP_SUCCESS) {
      ldap_perror( ld, "ldap_search_s" );
      exit(EXIT_FAILURE);
   }



for(entry = ldap_first_entry(ld, msg); entry != NULL; entry = ldap_next_entry(ld, entry)) {

      if((dn = ldap_get_dn(ld, entry)) != NULL) {
	//printf("Returned dn: %s\n", dn);
        tmp = dn;
        p = strtok (tmp,",");
        if(p){

        p = strtok(p,"=");  
        p = strtok(NULL, ","); 
	BlToSl(p);     
        //if (p)     
	//printf("p=%s\n",p);
	}        
        i++;
	json_object_array_add(my_array, json_object_new_int(i));
        json_object_array_put_idx(my_array, i, json_object_new_int(i));
	 struct json_object *obj = json_object_array_get_idx(my_array, i);
  
	json_object_object_add(my_object, json_object_to_json_string(obj), json_object_new_string(p));
         
	 ldap_memfree(dn);
      }

   }

   printf("%s\n", json_object_to_json_string(my_object));

   /* clean up */
   ldap_msgfree(msg);
   result = ldap_unbind_s(ld);

   if (result != 0) {
      fprintf(stderr, "ldap_unbind_s: %s\n", ldap_err2string(result));
      exit( EXIT_FAILURE );
   }

   return EXIT_SUCCESS;
}


//get username
int getUsername()
{
   LDAP *ld;
   struct json_object  *my_object,*my_array;
   const char* split = "CN=,";
   char *p;
   char*tmp;
   char *user;
   int i = 0;
   int nl ;
   int  result;
   int  auth_method = LDAP_AUTH_SIMPLE;
   int desired_version = LDAP_VERSION3;
   char *ldap_host = "WIN2K82";
   char *root_dn = "administrator@abc.com";
   char *root_pw = "123qwe!@#";

   BerElement* ber;
   LDAPMessage* msg;
   LDAPMessage* entry;

   char* base="CN=Users,dc=abc,dc=com";
   char* filter="(objectClass=user)";
   char* errstring;
   char* dn = NULL;
   char* attr;
   char** vals;
   

   if ((ld = ldap_init(ldap_host, LDAP_PORT)) == NULL ) {
      perror( "ldap_init failed" );
      exit( EXIT_FAILURE );
   }

   /* set the LDAP version to be 3 */
   if (ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &desired_version) != LDAP_OPT_SUCCESS)
   {
      ldap_perror(ld, "ldap_set_option");
      exit(EXIT_FAILURE);
   }

   if (ldap_bind_s(ld, root_dn, root_pw, auth_method) != LDAP_SUCCESS ) {
      ldap_perror( ld, "ldap_bind" );
      exit( EXIT_FAILURE );
   }

   if (ldap_search_s(ld, base, LDAP_SCOPE_SUBTREE, filter, NULL, 0, &msg) != LDAP_SUCCESS) {
      ldap_perror( ld, "ldap_search_s" );
      exit(EXIT_FAILURE);
   }


    my_object = json_object_new_object();
    my_array = json_object_new_array();
   
   int n = 0;//find return 
   /* Iterate through the returned entries */
   for(entry = ldap_first_entry(ld, msg); entry != NULL; entry = ldap_next_entry(ld, entry)) {

      if((dn = ldap_get_dn(ld, entry)) != NULL) {


         
	//printf("Returned dn: %s\n", dn);

        tmp = dn;
        p = strtok (tmp,",");
        if(p){

        p = strtok(p,"=");  
        p = strtok(NULL, ","); 
		
	BlToSl(p);     
        //if (p)     
	//printf("p=%s\n",p);
	}        
        i++;
	json_object_array_add(my_array, json_object_new_int(i));
        json_object_array_put_idx(my_array, i, json_object_new_int(i));
	 struct json_object *obj = json_object_array_get_idx(my_array, i);
  
	json_object_object_add(my_object, json_object_to_json_string(obj), json_object_new_string(p));
         
	 ldap_memfree(dn);
      }
	 
    
   }
   printf("%s\n", json_object_to_json_string(my_object));

   /* clean up */
   ldap_msgfree(msg);
   result = ldap_unbind_s(ld);

   if (result != 0) {
      fprintf(stderr, "ldap_unbind_s: %s\n", ldap_err2string(result));
      exit( EXIT_FAILURE );
   }

   return EXIT_SUCCESS;
}


int getGroupname()
{
   LDAP *ld;
   struct json_object  *my_object,*my_array;
   const char* split = "CN=,";
   char *p;
   char*tmp;
   char *user;
   int i = 0;
   int nl ;
   int  result;
   int  auth_method = LDAP_AUTH_SIMPLE;
   int desired_version = LDAP_VERSION3;
   char *ldap_host = "WIN2K82";
   char *root_dn = "administrator@abc.com";
   char *root_pw = "123qwe!@#";

   BerElement* ber;
   LDAPMessage* msg;
   LDAPMessage* entry;

   char* base="CN=Users,dc=abc,dc=com";
   char* filter="(objectClass=group)";
   char* errstring;
   char* dn = NULL;
   char* attr;
   char** vals;
   

   if ((ld = ldap_init(ldap_host, LDAP_PORT)) == NULL ) {
      perror( "ldap_init failed" );
      exit( EXIT_FAILURE );
   }

   /* set the LDAP version to be 3 */
   if (ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &desired_version) != LDAP_OPT_SUCCESS)
   {
      ldap_perror(ld, "ldap_set_option");
      exit(EXIT_FAILURE);
   }

   if (ldap_bind_s(ld, root_dn, root_pw, auth_method) != LDAP_SUCCESS ) {
      ldap_perror( ld, "ldap_bind" );
      exit( EXIT_FAILURE );
   }

   if (ldap_search_s(ld, base, LDAP_SCOPE_SUBTREE, filter, NULL, 0, &msg) != LDAP_SUCCESS) {
      ldap_perror( ld, "ldap_search_s" );
      exit(EXIT_FAILURE);
   }


    my_object = json_object_new_object();
   my_array = json_object_new_array();
   int n = 0;//find return 
   /* Iterate through the returned entries */
   for(entry = ldap_first_entry(ld, msg); entry != NULL; entry = ldap_next_entry(ld, entry)) {

      if((dn = ldap_get_dn(ld, entry)) != NULL) {
	//printf("Returned dn: %s\n", dn);
        tmp = dn;
        p = strtok (tmp,",");
        if(p){

        p = strtok(p,"=");  
        p = strtok(NULL, ",");  
	BlToSl(p);    
        //if (p)     
	//printf("p=%s\n",p);
	}        
        i++;
	json_object_array_add(my_array, json_object_new_int(i));
        json_object_array_put_idx(my_array, i, json_object_new_int(i));
	 struct json_object *obj = json_object_array_get_idx(my_array, i);
  
	json_object_object_add(my_object, json_object_to_json_string(obj), json_object_new_string(p));
         
	 ldap_memfree(dn);
      }
	 
    
   }
	//second search
    base="CN=Builtin,dc=abc,dc=com";
     
 if (ldap_search_s(ld, base, LDAP_SCOPE_SUBTREE, filter, NULL, 0, &msg) != LDAP_SUCCESS) {
      ldap_perror( ld, "ldap_search_s" );
      exit(EXIT_FAILURE);
   }



for(entry = ldap_first_entry(ld, msg); entry != NULL; entry = ldap_next_entry(ld, entry)) {

      if((dn = ldap_get_dn(ld, entry)) != NULL) {
	//printf("Returned dn: %s\n", dn);
        tmp = dn;
        p = strtok (tmp,",");
        if(p){

        p = strtok(p,"=");  
        p = strtok(NULL, ","); 
	BlToSl(p);     
        //if (p)     
	//printf("p=%s\n",p);
	}        
        i++;
	json_object_array_add(my_array, json_object_new_int(i));
        json_object_array_put_idx(my_array, i, json_object_new_int(i));
	 struct json_object *obj = json_object_array_get_idx(my_array, i);
  
	json_object_object_add(my_object, json_object_to_json_string(obj), json_object_new_string(p));
         
	 ldap_memfree(dn);
      }
	 
    
   }

   printf("%s\n", json_object_to_json_string(my_object));

   /* clean up */
   ldap_msgfree(msg);
   result = ldap_unbind_s(ld);

   if (result != 0) {
      fprintf(stderr, "ldap_unbind_s: %s\n", ldap_err2string(result));
      exit( EXIT_FAILURE );
   }

   return EXIT_SUCCESS;
}




int main( int argc, char *argv[] ) {
   

if(argc == 2)
{
        if(strcmp(argv[1],"user")==0) 
        {
              getUsername();
        }
        else if(strcmp(argv[1],"group")==0)
      {	
              getGroupname();
      }
      else if(strcmp(argv[1],"computer")==0)
      {	
            getComputorName();
      }
  }
else
{
	printf("error\n");
}

  return 1;
}









