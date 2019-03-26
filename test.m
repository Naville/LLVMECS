extern void* hikari_ecs(char* name);
#include <Foundation/Foundation.h>
@interface fooo:NSObject
+(void)test;
@end
@implementation fooo
+(void)test{

}
@end
int main(){
  void* ptr=hikari_ecs("fooo");
  printf("%p",ptr);
  return 0;
}
