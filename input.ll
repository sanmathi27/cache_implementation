; ModuleID = 'test_module'
source_filename = "test_module"

@myGlobalVar = common global i32 0, align 4

define i32 @main() {
entry:
  %0 = alloca i32, align 4
  store i32 0, i32* %0, align 4
  br label %block1

block1:                                            ; preds = %entry
  %1 = load i32, i32* %0, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, i32* %0, align 4
  br label %block2

block2:                                            ; preds = %block1
  %3 = load i32, i32* %0, align 4
  %4 = add nsw i32 %3, 2
  store i32 %4, i32* %0, align 4
  br label %block3

block3:                                            ; preds = %block2
  %5 = load i32, i32* %0, align 4
  %6 = add nsw i32 %5, 3
  store i32 %6, i32* %0, align 4
  ret i32 0
}
