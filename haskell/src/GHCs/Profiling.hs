module GHCs.Profiling where

f :: IO ()
f = do
  print ([1..] !! 99999)
  putStrLn "f"

g :: IO ()
g = do
  print ([1..] !! 999999)
  putStrLn "g"

main :: IO ()
main = do
  f
  g
