; ModuleID = 'input.ll'
source_filename = "test_module"

@myGlobalVar = common global i32 0, align 4
@myGlobalVar.1 = common global i32 0, align 4

define i32 @main() {
entry:
  store i32 0, ptr @myGlobalVar.1, align 4
  %0 = alloca i32, align 4
  store i32 0, ptr %0, align 4
  br label %block1

block1:                                           ; preds = %entry
  store i32 1, ptr @myGlobalVar.1, align 4
  %1 = load i32, ptr %0, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, ptr %0, align 4
  br label %block2

block2:                                           ; preds = %block1
  store i32 2, ptr @myGlobalVar.1, align 4
  %3 = load i32, ptr %0, align 4
  %4 = add nsw i32 %3, 2
  store i32 %4, ptr %0, align 4
  br label %block3

block3:                                           ; preds = %block2
  store i32 3, ptr @myGlobalVar.1, align 4
  %5 = load i32, ptr %0, align 4
  %6 = add nsw i32 %5, 3
  store i32 %6, ptr %0, align 4
  ret i32 0
}
